#include "ReturnFile.h"
#include "string.h"
#include "stdio.h"
#include "time.h"
#include <stdlib.h>

#define ALL_DATA_FILE_NAME ("AllData.txt")
#define PRIVATEDATA_NOT_CHANGE_VALUE ("ffffffffffffffff00000000")
#define BANK_RETURN_FILE ("bakReturnFile.bak")

ReturnFile::ReturnFile()
{
	m_unReturnFileType = E_NONE_TYPE;

	memset(m_acPlatFormID, 0, sizeof(m_acPlatFormID));

	Clearup();
}

ReturnFile::~ReturnFile()
{
}

void ReturnFile::SetPlatformID(char *pBuffer, int nLen)
{
	if (pBuffer != NULL && sizeof(m_acPlatFormID) > nLen)
	{
		memcpy(m_acPlatFormID, pBuffer, nLen);
	}
}

void ReturnFile::SetReturnFileType(unsigned int unReturnFileType)
{
	m_unReturnFileType |= unReturnFileType;
}

unsigned int ReturnFile::GetReturnFileType(void)
{
	return m_unReturnFileType;
}

unsigned int ReturnFile::ParseReturnData(unsigned char *pBuffer, unsigned int unLen)
{
	unsigned int unRet = 0;
	unsigned int unIndex = 0;
	unsigned int unDataLen = 0;
	char allReturnFile[512] = { 0 };

	do
	{
		Clearup();

		if (pBuffer == NULL || unLen == 0)
		{
			break;
		}

		if (m_unReturnFileType & E_CHIP_RT)
		{
			unsigned int unChipIDOrgLen = 0;
			unChipIDOrgLen = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			unIndex += 2;
			for (int ii = 0; ii < unChipIDOrgLen; ii++)//Hex
			{
				m_unChipID = m_unChipID * 256 + pBuffer[unIndex + ii];
				sprintf_s(&m_acChipID[ii * 2], sizeof(m_acChipID)-ii * 2, "%02x", pBuffer[unIndex + ii]);
			}
			unIndex += unChipIDOrgLen;
			sprintf_s(allReturnFile, sizeof(allReturnFile), "%s", m_acChipID);
		}

		if (m_unReturnFileType & E_SN_RT)
		{
			unsigned int unSNOrgLen = 0;
			unSNOrgLen = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			if (unSNOrgLen > sizeof(m_acSN))
			{
				break;
			}
			unIndex += 2;
			memcpy(m_acSN, &pBuffer[unIndex], unSNOrgLen);
			unIndex += unSNOrgLen;
			sprintf_s(&allReturnFile[strlen(allReturnFile)], sizeof(allReturnFile)-strlen(allReturnFile), ",%s", m_acSN);
		}

		if (m_unReturnFileType & E_MAC_RT)
		{
			unsigned int unMacOrgLen = 0;
			unMacOrgLen = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			if (unMacOrgLen > sizeof(m_acMac))
			{
				break;
			}
			unIndex += 2;
			memcpy(m_acMac, &pBuffer[unIndex], unMacOrgLen);
			unIndex += unMacOrgLen;
			sprintf_s(&allReturnFile[strlen(allReturnFile)], sizeof(allReturnFile)-strlen(allReturnFile), ",%s", m_acMac);
		}

		if (m_unReturnFileType & E_JOBID_RT)
		{
			unsigned int unPersonal = 0;
			unPersonal = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			if (unPersonal < 8)
			{
				break;
			}
			unIndex += 2;
			m_unVSCID = (unsigned int)((pBuffer[unIndex] << 24) | (pBuffer[unIndex + 1] << 16) | (pBuffer[unIndex + 2] << 8) | pBuffer[unIndex + 3]);

			unIndex += 4;
			m_unJobID = (unsigned int)((pBuffer[unIndex] << 24) | (pBuffer[unIndex + 1] << 16) | (pBuffer[unIndex + 2] << 8) | pBuffer[unIndex + 3]);

			unIndex += 4;
			memcpy(m_aucValidNum, &pBuffer[unIndex], unPersonal-8);
			m_unValidLen = unPersonal - 8;
			unIndex += m_unValidLen;
			sprintf_s(&allReturnFile[strlen(allReturnFile)], sizeof(allReturnFile)-strlen(allReturnFile), ",%u,%u", m_unVSCID, m_unJobID);
		}

		if (m_unReturnFileType & E_CA_SERIAL_NUM)
		{
			unsigned int unCardLen = (unsigned int)((pBuffer[unIndex] << 8) | pBuffer[unIndex + 1]);
			if ( unCardLen < 30 )
			{
				break;
			}
			unIndex += 2;
			for ( int ii = 0; ii < 8; ii++ )
			{
				sprintf_s(&m_acCASerialNum[ii*2], sizeof(m_acCASerialNum) - ii*2, "%02x", pBuffer[unIndex+ii]);
			}
			unIndex += 8;

			for (int jj = 0; jj < 4; jj++)
			{
				sprintf_s(&m_acCardID[jj * 2], sizeof(m_acCardID)-jj * 2, "%02x", pBuffer[unIndex + jj]);
			}
			unIndex += 4;

			memcpy(m_acCardSN, &pBuffer[unIndex], 16);
			unIndex += 16;
			sprintf_s(&allReturnFile[strlen(allReturnFile)], sizeof(allReturnFile)-strlen(allReturnFile), ",%s,%s,%s", m_acCASerialNum, m_acCardID, m_acCardSN);
		}
		else if (m_unReturnFileType & E_TSC_DATA)
		{
			unsigned int unTscLen = (unsigned int)((pBuffer[unIndex] << 8) | pBuffer[unIndex + 1]);
			if (unTscLen == 0)
			{
				break;
			}
			unIndex += 2;
			for (int ii = 0; ii < unTscLen; ii++)
			{
				sprintf_s(&m_acTscData[ii * 2], sizeof(m_acTscData)-ii * 2, "%02x", pBuffer[unIndex + ii]);
			}
			unIndex += unTscLen;
			sprintf_s(&allReturnFile[strlen(allReturnFile)], sizeof(allReturnFile)-strlen(allReturnFile), ",%s", m_acTscData);
		}

		//sprintf_s(allReturnFile, sizeof(allReturnFile), "%s,%s,%s", m_acChipID, m_acSN, m_acMac);
		FILE *pFile = NULL;
		fopen_s(&pFile, BANK_RETURN_FILE, "wb+");
		if (pFile != NULL)
		{
			fwrite(allReturnFile, sizeof(char), strlen(allReturnFile), pFile);

			fclose(pFile);
		}

		unRet = 1;
	} while (0);

	return unRet;
}

//ChipID+0x100000000+Digit
unsigned int ReturnFile::GetCDSN(unsigned int unChipID, char acCDSN[], unsigned int unLen)
{
	unsigned int unRet = 0;
	char acChipID[CHIP_ID_LEN] = { 0 };

	if (acCDSN != NULL)
	{
		sprintf_s(acChipID, sizeof(acChipID), "%u", unChipID);
		AdditionAB("4294967296", acChipID, acCDSN);
		acCDSN[strlen(acCDSN)] = CalculateCheckDigit(acCDSN, strlen(acCDSN));
		unRet = 1;
	}

	return unRet;
}

unsigned char ReturnFile::CalculateCheckDigit(char * pSN, unsigned char bLength)
{
	unsigned char bResult = 0, bDigit = 0;
	unsigned char CheckDigit = 0;

	while (bLength > 0)
	{
		bLength--;
		bDigit = (pSN[bLength] - '0') * (bLength % 2 + 1);
		bResult = bResult + (bDigit / 10) + (bDigit % 10);
	}

	bDigit = bResult % 10;
	CheckDigit = (unsigned char)(bDigit == 0 ? bDigit + '0' : 10 - bDigit + '0');

	return CheckDigit;
}

void ReturnFile::AdditionAB(char *a, char *b, char* c)
{
	char /*a[1000], b[1000], c[1001] = {0},*/ flag = 0;  /*c:存放结果，flag:进位信息*/
	int i, j, k, m, n, len; /*i：跟踪a的下标，j：跟踪b的下标，k：跟踪c的下标*/
	//scanf("%s%s", a, b);
	m = strlen(a);
	n = strlen(b);
	len = (m > n) ? m : n;
	/*相加时，先将右边对齐，即将i指向a的右端，j指向b的右端，
	然后i、j对齐，同步向左移动，直到其中一个到头为止*/
	for (i = m - 1, j = n - 1, k = len - 1; i >= 0 && j >= 0; i--, j--, k--)
	{
		c[k] = (a[i] - '0' + b[j] - '0' + flag) % 10 + '0';
		flag = (a[i] - '0' + b[j] - '0' + flag) / 10;
	}
	for (; i >= 0; i--, k--)  /*若a更长，即a没有处理完，处理a剩下的高位部分*/
	{
		c[k] = (a[i] - '0' + flag) % 10 + '0';
		flag = (a[i] - '0' + flag) / 10;
	}
	for (; j >= 0; j--, k--)  /*若b更长，则方法同上*/
	{
		c[k] = (b[j] - '0' + flag) % 10 + '0';
		flag = (b[j] - '0' + flag) / 10;
	}
	if (flag != 0) /*若最终的最高位进位信息不为0*/
	{
		for (k = len; k > 0; k--)
			c[k] = c[k - 1];
		c[0] = flag + '0';
	}
}

void ReturnFile::Clearup(void)
{
	memset(m_acSN, 0, sizeof(m_acSN));
	memset(m_acMac, 0, sizeof(m_acMac));
	memset(m_acChipID, 0, sizeof(m_acChipID));
	memset(m_aucValidNum, 0, sizeof(m_aucValidNum));
	memset(m_acCASerialNum, 0, sizeof(m_acCASerialNum));
	memset(m_acCardID, 0, sizeof(m_acCardID));
	memset(m_acCardSN, 0, sizeof(m_acCardSN));
	memset(m_acTscData, 0, sizeof(m_acTscData));
	m_unVSCID = 0;
	m_unJobID = 0;
	m_unValidLen = 0;
	m_unChipID = 0;
}

unsigned int ReturnFile::ConstructPrintData(void)
{
	FILE *pFILE = NULL;
	unsigned int unFinished = 0;
	unsigned int unLen = 0;
	char acStr[2*1024] = { 0 };

	memset(acStr, 0, sizeof(acStr));

	/*if (m_unReturnFileType & E_CHIP_RT)
	{
		unLen += GetChipIDData(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}*/
	if (m_unReturnFileType & E_SN_RT)
	{
		unLen += GetSNData(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	/*if (m_unReturnFileType & E_MAC_RT)
	{
		unLen += GetMACData(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}*/
	if (m_unReturnFileType & E_CDSN_RT)
	{
		unLen += GetCDSNData(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	/*if (m_unReturnFileType & E_PRIVATE_RT)
	{
		unLen += GetPrivateData(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_PIN_RT)
	{
		unLen += GetPINData(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_DIGCHIP_RT)
	{
		unLen += GetDigitChipIDData(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_JOBID_RT)
	{
		unLen += GetJobID(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_VSCID_RT)
	{
		unLen += GetVSCID(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_VALID_RT)
	{
		unLen += GetValidNum(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_STBID_RT)
	{
		unLen += GetSTBID(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_CA_SERIAL_NUM)
	{
		unLen += GetCASerialNum(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_CARD_ID)
	{
		unLen += GetCardID(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_CARD_SN)
	{
		unLen += GetCardSN(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}
	if (m_unReturnFileType & E_TSC_DATA)
	{
		unLen += GetTscData(&acStr[unLen], sizeof(acStr)-unLen);
		acStr[unLen++] = ',';
	}*/

	if (strlen(acStr) > 0 && acStr[0] != ',')
	{
		fopen_s(&pFILE, PRINT_FILE_NAME, "wb+");
		if (pFILE != NULL)
		{
			fwrite(acStr, sizeof(char), unLen, pFILE);

			fclose(pFILE);

			unFinished = 1;
		}

		//Save Data Castpal Self
		char acFileName[128] = { 0 };
		time_t t = time(&t);
		struct tm* current_time = localtime(&t);
		sprintf_s(acFileName, sizeof(acFileName), "%04d-%02d-%02d_%s", (1900 + current_time->tm_year), (1 + current_time->tm_mon), current_time->tm_mday, ALL_DATA_FILE_NAME);
		fopen_s(&pFILE, acFileName, "ab+");
		if (pFILE != NULL)
		{
			fseek(pFILE, 0, SEEK_END);
			memcpy((acStr + unLen), "\r\n", 2);
			unLen += 2;
			fwrite(acStr, sizeof(char), unLen, pFILE);

			fclose(pFILE);
		}
	}

	return unFinished;
}

unsigned int ReturnFile::CalculatePIN(char *pSN, int unLen)
{
	unsigned int unPIN = 0;

	if (pSN != NULL || unLen > 5)
	{
		unPIN = atoi((char*)&pSN[unLen - 5]);
		unPIN = unPIN / 10000 + unPIN % 10000;
		while (unPIN >= 10000)
		{
			unPIN = unPIN / 10000 + unPIN % 10000;
		}
		if (unPIN == 0)
		{
			unPIN = 10;
		}
	}

	return unPIN;
}

unsigned int ReturnFile::VerifyBarCode(unsigned char *pBuffer, int nLen, char *pBarCode, int nBarLen)
{
	unsigned int unRet = 0;

	if (ParseReturnData(pBuffer, nLen) == 1)
	{
		if (memcmp(pBarCode, m_acSN, nBarLen) == 0)
		{
			unRet = 1;
		}
	}

	return unRet;
}

unsigned int ReturnFile::GetValuesByType(int nType, char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	switch (nType)
	{
	case E_SN_RT:
		unRet = GetSNData(pBuffer, nLen);
		break;

	case E_CHIP_RT:
		unRet = GetChipIDData(pBuffer, nLen);
		break;

	case E_MAC_RT:
		unRet = GetMACData(pBuffer, nLen);
		break;

	case E_CDSN_RT:
		unRet = GetCDSNData(pBuffer, nLen);
		break;

	case E_PRIVATE_RT:
		unRet = GetPrivateData(pBuffer, nLen);
		break;

	case E_PIN_RT:
		unRet = GetPINData(pBuffer, nLen);
		break;

	case E_DIGCHIP_RT:
		unRet = GetDigitChipIDData(pBuffer, nLen);
		break;

	case E_JOBID_RT:
		unRet = GetJobID(pBuffer, nLen);
		break;

	case E_VSCID_RT:
		unRet = GetVSCID(pBuffer, nLen);
		break;

	case E_VALID_RT:
		unRet = GetValidNum(pBuffer, nLen);
		break;

	case E_STBID_RT:
		unRet = GetSTBID(pBuffer, nLen);
		break;

	case E_CA_SERIAL_NUM:
		unRet = GetCASerialNum(pBuffer, nLen);
		break;

	case E_CARD_ID:
		unRet = GetCardID(pBuffer, nLen);
		break;

	case E_CARD_SN:
		unRet = GetCardSN(pBuffer, nLen);
		break;

	case E_TSC_DATA:
		unRet = GetTscData(pBuffer, nLen);
		break;

	default:
		break;
	}

	return unRet;
}

unsigned int ReturnFile::GetSNData(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	m_unReturnFileType |= E_SN_RT;

	if (pBuffer != NULL && nLen > strlen(m_acSN))
	{
		memcpy(pBuffer, m_acSN, strlen(m_acSN));

		unRet = strlen(m_acSN);
	}

	return unRet;
}

unsigned int ReturnFile::GetChipIDData(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	m_unReturnFileType |= E_CHIP_RT;

	if (pBuffer != NULL && nLen > strlen(m_acChipID))
	{
		memcpy(pBuffer, m_acChipID, strlen(m_acChipID));

		unRet = strlen(m_acChipID);
	}

	return unRet;
}

unsigned int ReturnFile::GetMACData(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	m_unReturnFileType |= E_MAC_RT;

	if (pBuffer != NULL && nLen > strlen(m_acMac))
	{
		memcpy(pBuffer, m_acMac, strlen(m_acMac));

		unRet = strlen(m_acMac);
	}

	return unRet;
}

unsigned int ReturnFile::GetCDSNData(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;
	char acCDSN[64] = { 0 };

	m_unReturnFileType |= E_CDSN_RT;

	if (GetCDSN(m_unChipID, acCDSN, sizeof(acCDSN)) > 0)
	{
		if (pBuffer != NULL && nLen > strlen(acCDSN))
		{
			memcpy(pBuffer, acCDSN, strlen(acCDSN));

			unRet = strlen(acCDSN);
		}
	}

	return unRet;
}

unsigned int ReturnFile::GetPrivateData(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	m_unReturnFileType |= E_PRIVATE_RT;

	if (pBuffer != NULL && nLen > strlen(m_acChipID) + strlen(PRIVATEDATA_NOT_CHANGE_VALUE))
	{
		sprintf_s(pBuffer, nLen, "%s%s", PRIVATEDATA_NOT_CHANGE_VALUE, m_acChipID);

		unRet = strlen(m_acChipID) + strlen(PRIVATEDATA_NOT_CHANGE_VALUE);
	}

	return unRet;
}

unsigned int ReturnFile::GetPINData(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;
	unsigned int unPIN = CalculatePIN(m_acSN, strlen(m_acSN));

	m_unReturnFileType |= E_PIN_RT;

	if (unPIN > 0)
	{
		if (pBuffer != NULL && nLen > 4)
		{
			sprintf_s(pBuffer, nLen, "%04d", unPIN);

			unRet = 4;
		}
	}

	return unRet;
}

unsigned int ReturnFile::GetDigitChipIDData(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	m_unReturnFileType |= E_DIGCHIP_RT;

	if (pBuffer != NULL && nLen > strlen(m_acChipID))
	{
		sprintf_s(pBuffer, nLen, "%lld", m_unChipID);

		unRet = strlen(pBuffer);
	}

	return unRet;
}

unsigned int ReturnFile::GetJobID(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL && nLen > 0)
	{
		sprintf_s(pBuffer, nLen, "%u", m_unJobID);

		unRet = strlen(pBuffer);
	}

	return unRet;
}

unsigned int ReturnFile::GetVSCID(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL && nLen > 0)
	{
		sprintf_s(pBuffer, nLen, "%u", m_unVSCID);

		unRet = strlen(pBuffer);
	}

	return unRet;
}

unsigned int ReturnFile::GetValidNum(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL && nLen > m_unValidLen * 2)
	{
		for (int ii = 0; ii < m_unValidLen; ii++)
		{
			sprintf_s(&pBuffer[ii*2], nLen - ii * 2, "%02x", m_aucValidNum[ii]);
		}

		unRet = m_unValidLen * 2;
	}

	return unRet;
}

unsigned int ReturnFile::GetSTBID(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL)
	{
		sprintf_s(pBuffer, nLen, "%s%s", m_acPlatFormID, m_acChipID);

		unRet = strlen(pBuffer);
	}

	return unRet;
}

unsigned int ReturnFile::GetCASerialNum(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL)
	{
		memcpy( pBuffer, m_acCASerialNum, nLen );

		unRet = strlen(pBuffer);
	}

	return unRet;
}

unsigned int ReturnFile::GetCardID(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL)
	{
		memcpy(pBuffer, m_acCardID, nLen);

		unRet = strlen(pBuffer);
	}

	return unRet;
}

unsigned int ReturnFile::GetCardSN(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL)
	{
		memcpy(pBuffer, m_acCardSN, nLen);

		unRet = strlen(pBuffer);
	}

	return unRet;
}

unsigned int ReturnFile::GetTscData(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL)
	{
		memcpy(pBuffer, m_acTscData, nLen);

		unRet = strlen(pBuffer);
	}

	return unRet;
}

unsigned int ReturnFile::CompareReturnValue(int nType, char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (nType & E_CHIP_RT)
	{
		if (pBuffer != NULL &&  nLen == strlen(m_acChipID))
		{
			if (memcmp(pBuffer, m_acChipID, nLen) == 0)
			{
				unRet = 1;
			}
		}
	}
	else if (nType & E_MAC_RT)
	{
		if (pBuffer != NULL &&  nLen == strlen(m_acMac))
		{
			if (memcmp(pBuffer, m_acMac, nLen) == 0)
			{
				unRet = 1;
			}
		}
	}

	return unRet;
}