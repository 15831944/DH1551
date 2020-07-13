#include "UpgradeCommon.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "time.h"

#define CRC_FLAG 0xEDB88320

UpgradeCommon::UpgradeCommon()
{

}

UpgradeCommon::~UpgradeCommon()
{

}

unsigned int UpgradeCommon::IsIPValid(const char * pacStr, unsigned int unLength)
{
	unsigned int unRet = 0;
	unsigned int ii = 0;
	int nCount = 0;
	int nNumCount = 0;
	char acNum[8] = { 0 };

	for (ii = 0; ii<unLength; ii++)
	{
		if ((pacStr[ii] >= '0' && pacStr[ii] <= '9') || pacStr[ii] == '.')
		{
			if (pacStr[ii] == '.')
			{
				nCount++;
				nNumCount = 0;
				memset(acNum, 0, sizeof(acNum));
				if (nCount > 3)
				{
					break;
				}
			}
			else
			{
				acNum[nNumCount] = pacStr[ii];
				nNumCount++;
				if (nNumCount > 3 || atoi(acNum) > 255)
				{
					nCount = 0;
					break;
				}
			}
		}
	}

	if (nCount == 3)
	{
		unRet = 1;
	}

	return unRet;
}

DWORD UpgradeCommon::CalculateCRC32(BYTE * pbData, unsigned int unLength)
{
	unsigned int ii = 0;
	unsigned int jj = 0;
	DWORD dwCRC = 0;
	DWORD dwCrc32 = 0xffffffff;

	for (ii = 0; ii<unLength; ii++)
	{
		for (jj = 0; jj<8; jj++)
		{
			if (((pbData[ii] >> (7 - jj)) & 0x01))
				dwCrc32 ^= 1;

			if (dwCrc32 & 1)
				dwCrc32 = ((dwCrc32 >> 1) ^ CRC_FLAG);
			else
				dwCrc32 >>= 1;
		}
	}

	for (ii = 0; ii<32; ii++)
	{
		if ((dwCrc32 >> ii) & 1)
		{
			dwCRC <<= 1;
			dwCRC |= 1;
		}
		else
		{
			dwCRC <<= 1;
		}
	}

	return dwCRC;
}

unsigned int UpgradeCommon::CheckCRC32(unsigned char aucStr[], unsigned int unLength)
{
	DWORD dwCRC32 = 0;
	DWORD dwOrgCRC32 = 0;
	unsigned int unRet = 0;
	unsigned int unDataIndex = 0;
	unsigned int unDataLen = 0;

	if (unLength < MIN_DATA_SIZE)
	{
		return unRet;
	}

	//HEDA_FLAG(1B) + TYPE(2B)
	unDataIndex += 3;

	unDataLen = (aucStr[unDataIndex] << 8) | aucStr[unDataIndex + 1];
	unDataIndex += (unDataLen + 2);

	dwCRC32 = ((DWORD)aucStr[unDataIndex] << 24) | ((DWORD)aucStr[unDataIndex + 1] << 16) | ((DWORD)aucStr[unDataIndex + 2] << 8) | ((DWORD)aucStr[unDataIndex + 3]);
	dwOrgCRC32 = CalculateCRC32(aucStr, unDataLen + 5);
	if (dwOrgCRC32 == dwCRC32)
	{
		unRet = 1;
	}

	return unRet;
}

void UpgradeCommon::WriteLog(char * format, ...)
{
	static char buf[2048] = { 0 };
	FILE * pLogFile = NULL;
	size_t len;
	va_list args;

	time_t t = time(&t);
	struct tm* current_time = localtime(&t);
	sprintf_s(buf, sizeof(buf), "%04d/%02d/%02d", (1900 + current_time->tm_year), (1 + current_time->tm_mon), current_time->tm_mday);
	len = strlen(buf);

	va_start(args, format);
	vsprintf(&buf[len], format, args);
	va_end(args);

	len = strlen(buf);

	pLogFile = fopen("UpgradLog.txt", "a+");
	if (pLogFile != NULL)
	{
		fwrite(buf, 1, len - 1, pLogFile);
		fclose(pLogFile);
	}
}