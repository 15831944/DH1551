#include "ConaxPersonalize.h"
#include "string.h"
#include "HttpClient/DebugLog.h"
#include "HttpClient/HttpClient.h"

#define VSCDATA_TAG  (0xCD)
#define VSCDATA_CMD_TAG  (0xCE)

#define CMD_MAX_CNT (257)

#define Isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#define PERSONALIZE_FILE	("PersonalData.txt")

char*strtok_r(char*s, const char*delim, char**ptrptr)
{
	char*tmp = 0;

	if (ptrptr == NULL)
		return NULL;

	if (NULL == s)
	{
		s = *ptrptr;
	}

	s += strspn(s, delim);        /* overread leading delimiter */
	if (*s)
	{
		tmp = s;
		s += strcspn(s, delim);
		if (*s)
		{
			*s++ = 0;    /* not the end ? => terminate it */
		}
	}

	*ptrptr = s;

	return tmp;
}

static int trimwhitespace(char *in, char *out)
{
	char *s, *d;

	s = in;
	d = out;

	while (*s != 0) {
		if (!Isspace(*s) && *s != '\"') {
			*d = *s;
			d++;
		}
		s++;
	}

	return (d - out);
}

static int hexstring_to_bytearray(const char *hexstr, unsigned char *buf, int len)
{
	int i = 0;
	const char *pos = hexstr;

	/* WARNING: no sanitization or error-checking whatsoever */
	for (i = 0; i < len; i++) {
		sscanf(pos, "%2hhx", &buf[i]);

		pos += 2;
	}

	return 0;
}

static void hexprintf(char *txt, unsigned char *p, int len)
{
	int i = 0;

	PrintfLog("%s[len:%d]:", txt, len);
	for (i = 0; i<len; i++)
	{
		PrintfLog("%02x ", p[i]);
		if ((i + 1) % 32 == 0)
		{
			PrintfLog("\r\n");
		}
	}
	PrintfLog("\r\n");
}

static unsigned int GetMPegCRC32(unsigned char* pucData, unsigned int nLen)
{
	unsigned char cDataBeingDone;
	unsigned char cTempData;
	unsigned char *pucTemp = 0;
	int jj = 0;
	unsigned int kk = 0;
	int XOR_REG = 0;
	int PN_REG = -1;

	if (pucData == 0)
	{
		return -1;
	}

	pucTemp = pucData;

	for (kk = 0; kk<nLen; kk++)
	{
		cDataBeingDone = pucTemp[kk];

		for (jj = 0; jj<8; jj++)
		{
			cTempData = cDataBeingDone;
			cTempData = (cTempData >> 7) + (unsigned char)(PN_REG >> 31);

			if ((cTempData & 0x01) == 0x01)
			{
				XOR_REG = 0x2608edb;
			}
			else
			{
				XOR_REG = 0;
			}

			PN_REG = PN_REG^XOR_REG;
			PN_REG = (PN_REG << 1);

			if ((cTempData & 0x01) == 0x01)
			{
				PN_REG |= 0x01;
			}
			else
			{
				PN_REG &= 0xfffffffe;
			}

			cDataBeingDone = (unsigned char)(cDataBeingDone << 1);
		}

	}

	return (PN_REG);
}

ConaxPersonalize::ConaxPersonalize()
{
	memset(m_acIP, 0, sizeof(m_acIP));

	m_unPort = 0;

	m_unJobID = 0;
	m_unFirstVscID = 0;
	m_unLastVscID = 0;
	m_unNumberofIssuedUnits = 0;

	memset(m_pInputCSVFilePath, 0, sizeof(m_pInputCSVFilePath));
	memset(m_pJobfilepath, 0, sizeof(m_pJobfilepath));
}

ConaxPersonalize::~ConaxPersonalize()
{
}

int ConaxPersonalize::SetNetworkInfo(char *pIP, int nLen, int nPort)
{
	int nRet = 0;

	if (pIP != NULL && nPort > 0)
	{
		memcpy(m_acIP, pIP, nLen);

		m_unPort = nPort;

		nRet = 1;
	}

	return nRet;
}


void ConaxPersonalize::SetInputCSVFilePath(char *path)
{
	memset(m_pInputCSVFilePath, 0, sizeof(m_pInputCSVFilePath));
	strcpy(m_pInputCSVFilePath, path);
}

int ConaxPersonalize::InitPersonalize(void)
{
	int nRet = 0;

	if (m_unJobID == 0)
	{
		if (m_JobFileManage.InitConfig())
		{
			if (m_JobFileManage.GetCurJobFileName(m_pJobfilepath, sizeof(m_pJobfilepath)) > 0)
			{
				if (InitJobFile(m_pJobfilepath, strlen(m_pJobfilepath)) > 0)
				{
					nRet = 1;
				}
			}
		}
	}
	else
	{
		nRet = 1;
	}

	return nRet;
}

int ConaxPersonalize::LoadJobfile(void)
{
	int nRet = 0;

	if ((m_unLastVscID + 1 - m_unFirstVscID) == m_unNumberofIssuedUnits)//next job
	{
		//DeleteJobFile();

		memset(m_pJobfilepath, 0, sizeof(m_pJobfilepath));

		if (m_JobFileManage.GetNextJobFileName(m_pJobfilepath, sizeof(m_pJobfilepath)) > 0)
		{
			if (InitJobFile(m_pJobfilepath, strlen(m_pJobfilepath)) > 0)
			{
				nRet = 1;
			}
		}
	}
	else if ((m_unLastVscID + 1 - m_unFirstVscID) > m_unNumberofIssuedUnits)//exist job
	{
		nRet = 1;
	}

	return nRet;
}

int ConaxPersonalize::GetHomePage(void)
{
	int nRt = 0;
	CHttpClient httpClient;
	char cOptional[128];
	unsigned char pResponse[10240];
	unsigned int unHttpLen = 0;

	memset(pResponse, 0, sizeof(pResponse));
	memset(cOptional, 0, sizeof(cOptional));
	sprintf(cOptional, "http://%s:%d", m_acIP, m_unPort);

	nRt = httpClient.HttpURLGet(cOptional, pResponse, &unHttpLen);

	PrintfLog("[homepage_GET %d] nRt:%d \r\n", __LINE__, nRt);
	if (0 == nRt)
	{
		PrintfLog("[homepage_GET %d] pResponse:\r\n[%s] \r\n", __LINE__, pResponse);
	}

	return nRt;
}

int ConaxPersonalize::InitJobFile(char *pFileName, int nLen)
{
	int nRt = -1;
	CHttpClient httpClient;
	static unsigned char pucResponse[0x10000];
	unsigned int unResponseLen = 0x10000;
	char *p = 0;
	char cHeaderInfo[4096];

	memset(pucResponse, 0, sizeof(pucResponse));
	memset(cHeaderInfo, 0, sizeof(cHeaderInfo));
	
	nRt = httpClient.UploadFile(m_acIP, m_unPort, "/initjob", pFileName, pucResponse, &unResponseLen, cHeaderInfo);

	PrintfLog("[ initjob_POST ] nRt:%d unResponseLen:%d \r\n", nRt, unResponseLen);

	p = (char*)pucResponse;

	if (0 == nRt)
	{
		int total = 0;
		char *pTemp = 0;

		m_unJobID = 0;
		m_unFirstVscID = 0;
		m_unLastVscID = 0;
		m_unNumberofIssuedUnits = 0;

		PrintfLog("[ initjob_POST ] pResponse[%s] \r\n", p);

		//hexprintf("old", buf, strlen(buf));

		/* remove all white spaces to make parsing easier */
		total = trimwhitespace(p, p);
		p[total - 2] = '\0';

		//hexprintf("new", buf, strlen(buf));
		//PrintfLog("NewFile:%s\r\n", p);

		pTemp = strstr(p, "jobId");
		if (0 != pTemp)
		{
			pTemp += strlen("jobId") + 1;
		}
		m_unJobID = strtoul(pTemp, 0, 10);
		PrintfLog("===m_unJobID:%u\r\n", m_unJobID);

		pTemp = strstr(p, "firstVscId");
		if (0 != pTemp)
		{
			pTemp += strlen("firstVscId") + 1;
		}
		m_unFirstVscID = strtoul(pTemp, 0, 10);
		PrintfLog("===m_unFirstVscID:%u\r\n", m_unFirstVscID);

		pTemp = strstr(p, "lastVscId");
		if (0 != pTemp)
		{
			pTemp += strlen("lastVscId") + 1;
		}
		m_unLastVscID = strtoul(pTemp, 0, 10);
		PrintfLog("===m_unLastVscID:%u\r\n", m_unLastVscID);

		pTemp = strstr(p, "numberOfIssuedUnits");
		if (0 != pTemp)
		{
			pTemp += strlen("numberOfIssuedUnits") + 1;
		}
		m_unNumberofIssuedUnits = strtoul(pTemp, 0, 10);
		PrintfLog("===m_unNumberofIssuedUnits:%u\r\n", m_unNumberofIssuedUnits);

		nRt = 1;
	}

	return nRt;
}

int ConaxPersonalize::DeleteJobFile(void)
{
	int nRt = 0;
	CHttpClient httpClient;
	char cOptional[128];
	unsigned char pResponse[10240];
	unsigned int unResponseLen = 0;

	memset(pResponse, 0, sizeof(pResponse));
	memset(cOptional, 0, sizeof(cOptional));
	sprintf(cOptional, "http://%s:%d/deletejob?jobid=%d", m_acIP, m_unPort, m_unJobID);

	if (0 == m_unJobID)
	{
		//AfxMessageBox("Please Load JobFile firstly");
	}
	else
	{
		nRt = httpClient.HttpURLGet(cOptional, pResponse, &unResponseLen);

		PrintfLog("[deletejob_GET %d] nRt:%d \r\n", __LINE__, nRt);

		if (0 == nRt)
		{
			int total = 0;
			char *pTemp = 0;
			unsigned int unDeleteJobID = 0;

			PrintfLog("[deletejob_GET %d] pResponse:[%s]  \r\n", __LINE__, pResponse);

			pTemp = (char*)&pResponse[0];
			/* remove all white spaces to make parsing easier */
			total = trimwhitespace(pTemp, pTemp);
			pTemp[total - 1] = '\0';

			//hexprintf("new", buf, strlen(buf));
			//PrintfLog("NewFile:%s\n", pTemp);

			/* locate start of persoCommands */
			pTemp = strstr(pTemp, "deletedJobId");
			if (0 != pTemp)
			{
				pTemp += strlen("deletedJobId") + 1;
			}

			unDeleteJobID = strtoul(pTemp, 0, 10);
			PrintfLog("===unDeleteJobID:%u\n", unDeleteJobID);
		}
	}

	return nRt;
}

int ConaxPersonalize::RequestEncrytionKey(unsigned int unChipID, unsigned char *pucSWEK, unsigned int *punSWEKLen)
{
	int nRt = 0;
	CHttpClient httpClient;
	char cOptional[128];
	unsigned char *p = 0;
	unsigned char pResponse[10240];
	unsigned int unResponseLen = 0;

	m_unChipID = unChipID;

	memset(pResponse, 0, sizeof(pResponse));
	memset(cOptional, 0, sizeof(cOptional));
	sprintf(cOptional, "http://%s:%d/getswek?jobid=%d&chipid=%d", m_acIP, m_unPort, m_unJobID, m_unChipID);

	nRt = httpClient.HttpURLGet(cOptional, pResponse, &unResponseLen);

	PrintfLog("[getswek_GET %d] nRt:%d \r\n", __LINE__, nRt);

	if (0 == nRt)
	{
		int total = 0;
		char *pTemp = 0;

		PrintfLog("[getswek_GET %d] pResponse:[%s] \r\n", __LINE__, pResponse);
		//hexprintf("old", buf, strlen(buf));

		pTemp = (char*)&pResponse[0];

		/* remove all white spaces to make parsing easier */
		total = trimwhitespace(pTemp, pTemp);
		pTemp[total - 1] = '\0';

		//hexprintf("new", buf, strlen(buf));
		//PrintfLog("NewFile:%s\r\n", pTemp);

		pTemp = strstr(pTemp, ":");
		if (0 != pTemp)
		{
			pTemp += 1;
			hexstring_to_bytearray(pTemp, pucSWEK, strlen(pTemp));
			*punSWEKLen = (strlen(pTemp) - 0) / 2;
			hexprintf("swek", pucSWEK, *punSWEKLen);

			nRt = 1;
		}
		else
		{
			nRt = -1;
		}
	}
	else
	{
		m_unJobID = 0;
	}

	return nRt;
}

int ConaxPersonalize::RequestVSCData(unsigned char *pvscdata, unsigned int *punvscdataLen)
{
	int nRt = 0;
	CHttpClient httpClient;
	char cOptional[128];
	unsigned int strLength = 0;
	int i = 0;
	int pos = 0;
	FILE *fp = 0;
	char vscfname[128];
	CString msg;
	unsigned char pResponse[10240];
	unsigned int unResponseLen = 0;
	int dataFromStoreFile = 0;

	memset(pResponse, 0, sizeof(pResponse));
	memset(cOptional, 0, sizeof(cOptional));
	sprintf(cOptional, "http://%s:%d/getvscdata?jobid=%d&chipid=%d", m_acIP, m_unPort, m_unJobID, m_unChipID);
	sprintf(vscfname, "%s/VSCData_%u.txt", JOBDIR, m_unChipID);

	if ((fp = fopen(vscfname, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		unResponseLen = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		fread(pResponse, 1, unResponseLen, fp);
		fclose(fp);
		fp = 0;
		dataFromStoreFile = 1;
	}
	else
	{
		nRt = httpClient.HttpURLGet(cOptional, pResponse, &unResponseLen);
		dataFromStoreFile = 0;
	}

	PrintfLog("[getvscdata_GET %d] nRt:%d  --------------dataFromStoreFile:%d ----------\r\n", __LINE__, nRt, dataFromStoreFile);

	if (0 == nRt)
	{
		CString msg;

		int total = 0;
		char *saveptr;
		char *pch;
		unsigned char cmd_data[512];
		int cmd_len = 0;
		char *pTemp = 0;
		unsigned int VSCID = 0;
		char *pdataTemp = (char*)&pResponse[0];

		PrintfLog("[getvscdata_GET %d] pResponse:%s \r\n", __LINE__, pdataTemp);

		// 缓存vscdata,vscdata使用一个就会减少一个,这里需要缓存,正式环境需要存到数据库
		if (0 == dataFromStoreFile)
		{
			if ((fp = fopen(vscfname, "w")) == NULL)
			{
				PrintfLog("ERROR: Can't open m_pCSVFilepath file:%s\r\n", vscfname);
				//AfxMessageBox("ERROR: Can't open fname file");
			}
			else
			{
				fwrite(pResponse, 1, unResponseLen, fp);
				PrintfLog("Write %s OK\r\n", vscfname);
				fclose(fp); /*关闭文件*/
			}
		}

		/* remove all white spaces to make parsing easier */
		total = trimwhitespace(pdataTemp, pdataTemp);
		pdataTemp[total - 2] = '\0';

		//hexprintf("new", buf, strlen(buf));
		//PrintfLog("NewFile:%s\r\n", pdataTemp);

		pTemp = strstr(pdataTemp, "vscId");
		if (0 != pTemp)
		{
			pTemp += strlen("vscid") + 1;
		}

		VSCID = strtoul(pTemp, 0, 10);
		PrintfLog("===VSCID:%u\r\n", VSCID);

		pvscdata[pos++] = VSCDATA_TAG;
		pvscdata[pos++] = 0; // len
		pvscdata[pos++] = 0; // len
		pvscdata[pos++] = 0; // len
		pvscdata[pos++] = 0; // len
		pvscdata[pos++] = (VSCID & 0xFF000000) >> 24; // VSCID
		pvscdata[pos++] = (VSCID & 0x00FF0000) >> 16; // VSCID
		pvscdata[pos++] = (VSCID & 0x0000FF00) >> 8; // VSCID
		pvscdata[pos++] = (VSCID & 0x000000FF); // VSCID
		pvscdata[pos++] = (m_unJobID & 0xFF000000) >> 24; // JOBID
		pvscdata[pos++] = (m_unJobID & 0x00FF0000) >> 16; // JOBID
		pvscdata[pos++] = (m_unJobID & 0x0000FF00) >> 8; // JOBID
		pvscdata[pos++] = (m_unJobID & 0x000000FF); // JOBID

		/* locate start of persoCommands */
		pdataTemp = strstr(pdataTemp, "[");
		if (0 != pdataTemp)
		{
			pdataTemp = pdataTemp + 1;
			/* split into list of perso commands */
			pch = strtok_r(pdataTemp, ",", &saveptr);
			while (pch != NULL)
			{
				cmd_len = strlen(pch) / 2;

				hexstring_to_bytearray(&pch[0], cmd_data, cmd_len);

				//cmd_len = strlen(pch) / 2;


				// hexprintf("cmd", cmd_data, cmd_len);

				pch = strtok_r(NULL, ",", &saveptr);

				if (i >= CMD_MAX_CNT)
				{
					PrintfLog("Cmd Count ERROR !!! \r\n");
					break;
				}

				pvscdata[pos++] = VSCDATA_CMD_TAG;
				pvscdata[pos++] = (cmd_len & 0xFF00) >> 8;
				pvscdata[pos++] = (cmd_len & 0x00FF);
				memcpy(&pvscdata[pos], cmd_data, cmd_len);
				pos += cmd_len;

				i++;
			}
		}

		pos += 4; //crc 4

		pvscdata[1] = ((pos - 5) & 0xFF000000) >> 24; // len
		pvscdata[2] = ((pos - 5) & 0x00FF0000) >> 16; // len
		pvscdata[3] = ((pos - 5) & 0x0000FF00) >> 8; // len
		pvscdata[4] = ((pos - 5) & 0x000000FF); // len

		unsigned int crc = 0;
		crc = GetMPegCRC32(pvscdata, (pos - 4));

		pvscdata[pos - 4] = (crc & 0xFF000000) >> 24; // crc
		pvscdata[pos - 3] = (crc & 0x00FF0000) >> 16; // crc
		pvscdata[pos - 2] = (crc & 0x0000FF00) >> 8; // crc
		pvscdata[pos - 1] = (crc & 0x000000FF); // crc

		PrintfLog("[getvscdata_GET %d] end len:%d, crc:0x%x \r\n", __LINE__, pos, crc);

		*punvscdataLen = pos;

		RecordPersonalizeData(m_unChipID, m_unJobID, VSCID);

		nRt = 1;
	}
	else
	{
		m_unJobID = 0;
	}

	return nRt;
}

int ConaxPersonalize::PrepareReportFile(unsigned char *pReport, unsigned int *punLen, char *filename)
{
	int nRt = 0;
	CHttpClient httpClient;
	char cHeaderInfo[4096];
	char *p = 0;

	memset(cHeaderInfo, 0, sizeof(cHeaderInfo));

	nRt = httpClient.UploadFile(m_acIP, m_unPort, "/preparereport", m_pInputCSVFilePath, pReport, punLen, cHeaderInfo);
	PrintfLog("[preparereport_POST %d] nRt:%d report dataLen:%d\r\n", __LINE__, nRt, *punLen);

	p = strstr(cHeaderInfo, "VSC_rep");
	if (0 != p)
	{
		memcpy(filename, p, 35);
	}

	return nRt;
}

int ConaxPersonalize::RecordPersonalizeData(unsigned unChipID, unsigned int unJobID, unsigned int unVSCID)
{
	int nRet = 0;
	char acRecord[256] = { 0 };

	sprintf_s(acRecord, sizeof(acRecord), "%u,%u,%u\r\n", unChipID, unJobID, unVSCID);
	if (strlen(acRecord) > 0)
	{
		FILE *pFile = NULL;

		fopen_s(&pFile, PERSONALIZE_FILE, "ab+");
		if (pFile != NULL)
		{
			fwrite(acRecord, sizeof(char), strlen(acRecord), pFile);

			fclose(pFile);

			nRet = 1;
		}
	}

	return nRet;
}