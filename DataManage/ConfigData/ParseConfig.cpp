#include "ParseConfig.h"
#include "../CastpalServerData/CastpalServerData.h"
#include "stdio.h"
#include "string.h"

ParseConfig::ParseConfig()
{
	memset(&m_ConfigData, 0, sizeof(m_ConfigData));
}

ParseConfig::~ParseConfig()
{
}

int ParseConfig::SetConfigData(XMLNode *pTempXMLNode)
{
	int nRet = 1;

	nRet &= pTempXMLNode->GetValue("BARCODE", strlen("BARCODE"), &m_ConfigData.m_BarCode);
	nRet &= pTempXMLNode->GetValue("SERIAL", strlen("SERIAL"), m_ConfigData.m_acSerial, sizeof(m_ConfigData.m_acSerial));
	nRet &= pTempXMLNode->GetValue("SERVERIP", strlen("SERVERIP"), m_ConfigData.m_acServerIP, sizeof(m_ConfigData.m_acServerIP));
	nRet &= pTempXMLNode->GetValue("SERVERPORT", strlen("SERVERPORT"), &m_ConfigData.m_ServerPort);
	nRet &= pTempXMLNode->GetValue("LOCALIP", strlen("LOCALIP"), m_ConfigData.m_acLocalIP, sizeof(m_ConfigData.m_acLocalIP));
	nRet &= pTempXMLNode->GetValue("LOCALPORT", strlen("LOCALPORT"), &m_ConfigData.m_LocalPort);
	nRet &= pTempXMLNode->GetValue("PERSONALIP", strlen("PERSONALIP"), m_ConfigData.m_acPersonalIP, sizeof(m_ConfigData.m_acPersonalIP));
	nRet &= pTempXMLNode->GetValue("PERSONALPORT", strlen("PERSONALPORT"), &m_ConfigData.m_PersonalPort);
	nRet &= pTempXMLNode->GetValue("ORDERNUM", strlen("ORDERNUM"), m_ConfigData.m_acOrderNum, sizeof(m_ConfigData.m_acOrderNum));
	nRet &= pTempXMLNode->GetValue(&m_ConfigData.m_nSN, "SN", strlen("SN"));
	nRet &= pTempXMLNode->GetValue("SN_LEN", strlen("SN_LEN"), &m_ConfigData.m_nSNLen);
	nRet &= pTempXMLNode->GetValue(&m_ConfigData.m_nHDCP, "HDCP", strlen("HDCP"));
	nRet &= pTempXMLNode->GetValue(&m_ConfigData.m_nMAC, "MAC", strlen("MAC"));
	nRet &= pTempXMLNode->GetValue(&m_ConfigData.m_nPEF, "PEF", strlen("PEF"));
	nRet &= pTempXMLNode->GetValue(&m_ConfigData.m_nPersonal, "PERSONAL", strlen("PERSONAL"));
	nRet &= pTempXMLNode->GetValue("CONNECT", strlen("CONNECT"), &m_ConfigData.m_nConnType);
	nRet &= pTempXMLNode->GetValue("PLATID", strlen("PLATID"), m_ConfigData.m_acPlatID, sizeof(m_ConfigData.m_acPlatID));
	nRet &= pTempXMLNode->GetValue("PROJECT", strlen("PROJECT"), m_ConfigData.m_acProject, sizeof(m_ConfigData.m_acProject));
	nRet &= pTempXMLNode->GetValue("TOOLMODE", strlen("TOOLMODE"), &m_ConfigData.m_nToolMode);

	if (nRet == 1)
	{
		m_ConfigData.m_nDataType = (m_ConfigData.m_nSN * E_DATATYPE_SN) + (m_ConfigData.m_nMAC * E_DATATYPE_MAC) + (m_ConfigData.m_nHDCP * E_DATATYPE_HDCP);
		m_ConfigData.m_Status = 1;
	}

	return nRet;
}

unsigned int ParseConfig::GetConfigData(ConfigData & csConfigData)
{
	unsigned int unRet = 0;

	if (m_ConfigData.m_Status == 1)
	{
		memcpy(&csConfigData, &m_ConfigData, sizeof(ConfigData));

		unRet = 1;
	}

	return unRet;
}

unsigned int ParseConfig::ReadConfigFile(char *pFileName, int nLen)
{
	unsigned int unRet = 0;
	NodeList tempNodeList;
	XMLNode *pTempXMLNode = NULL;

	if (pFileName != NULL && nLen > 0)
	{
		if (tempNodeList.ConstructNodeListByFile(pFileName, nLen))
		{
			pTempXMLNode = tempNodeList.GetXMLNode();
			if (pTempXMLNode != NULL)
			{
				unRet = SetConfigData(pTempXMLNode);
			}
		}
	}

	return unRet;
}

unsigned int ParseConfig::WriteConfigFile(char *pFileName, int nLen, ConfigData csConfigData)
{
	unsigned int unRet = 0;
	int nIndex = 0;
	char acConfig[4 * 1024] = { 0 };
	char acCheck[][16] = { "False", "True" };

	memcpy(&m_ConfigData, &csConfigData, sizeof(ConfigData));

	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<BARCODE>%d</BARCODE>\r\n", csConfigData.m_BarCode);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<SERIAL>%s</SERIAL>\r\n", csConfigData.m_acSerial);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<SERVERIP>%s</SERVERIP>\r\n", csConfigData.m_acServerIP);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<SERVERPORT>%u</SERVERPORT>\r\n", csConfigData.m_ServerPort);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<LOCALIP>%s</LOCALIP>\r\n", csConfigData.m_acLocalIP);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<LOCALPORT>%u</LOCALPORT>\r\n", csConfigData.m_LocalPort);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<PERSONALIP>%s</PERSONALIP>\r\n", csConfigData.m_acPersonalIP);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<PERSONALPORT>%u</PERSONALPORT>\r\n", csConfigData.m_PersonalPort);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<ORDERNUM>%s</ORDERNUM>\r\n", csConfigData.m_acOrderNum);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<SN>%s</SN>\r\n", acCheck[csConfigData.m_nSN]);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<SN_LEN>%d</SN_LEN>\r\n", csConfigData.m_nSNLen);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<HDCP>%s</HDCP>\r\n", acCheck[csConfigData.m_nHDCP]);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<MAC>%s</MAC>\r\n", acCheck[csConfigData.m_nMAC]);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<PEF>%s</PEF>\r\n", acCheck[csConfigData.m_nPEF]);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<PERSONAL>%s</PERSONAL>\r\n", acCheck[csConfigData.m_nPersonal]);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<CONNECT>%d</CONNECT>\r\n", csConfigData.m_nConnType);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<PLATID>%s</PLATID>\r\n", csConfigData.m_acPlatID);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<PROJECT>%s</PROJECT>\r\n", csConfigData.m_acProject);
	sprintf_s(&acConfig[strlen(acConfig)], sizeof(acConfig)-strlen(acConfig), "<TOOLMODE>%d</TOOLMODE>", csConfigData.m_nToolMode);

	FILE *pFile = NULL;
	fopen_s(&pFile, pFileName, "wb");
	if (pFile != NULL)
	{
		fwrite(acConfig, sizeof(char), strlen(acConfig), pFile);

		fclose(pFile);

		unRet = 1;
	}

	return unRet;
}

unsigned int ParseConfig::SaveComChange(char * pComName, char *pFileName, int nLen)
{
	unsigned int unRet = 0;
	int ii = 0;
	FILE * pFile = NULL;
	char * p = NULL;
	char acData[1024 * 2] = { 0 };
	char acStr[1024 * 2] = { 0 };

	if (pComName == NULL || pFileName == NULL)
	{
		return unRet;
	}

	fopen_s(&pFile, pFileName, "rb");
	if (pFile == NULL)
	{
		return unRet;
	}

	if (fread_s(acData, sizeof(acData), sizeof(char), sizeof(acData), pFile) > 0)
	{
		if (pFile != NULL)
		{
			fclose(pFile);
		}

		for (ii = 0; ii < strlen(acData) - 2; ii++)
		{
			if (acData[ii] == 'C' && acData[ii + 1] == 'O' && acData[ii + 2] == 'M')
			{
				break;
			}
		}

		if (ii < strlen(acData))
		{
			p = strstr(acData, "</SERIAL>");
			if (p != NULL)
			{
				memcpy(acStr, p, sizeof(acStr));
				memcpy(&acData[ii], pComName, sizeof(acData)-ii - 1);
				ii += strlen(pComName);
				memcpy(&acData[ii], acStr, sizeof(acData)-ii - 1);

				fopen_s(&pFile, pFileName, "wb");
				if (pFile == NULL)
				{
					return unRet;
				}
				fwrite(acData, sizeof(char), strlen(acData), pFile);

				unRet = 1;
			}
		}
	}

	if (pFile != NULL)
	{
		fclose(pFile);
	}

	return unRet;
}