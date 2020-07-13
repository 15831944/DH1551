#include "JobFileManage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define JOBINDEX ("JOBINDEX")
#define JOBNAME ("JobFile.cxf")
#define JOBCONFIG ("Source/JobConfig.txt")

JobFileManage::JobFileManage()
{
}

JobFileManage::~JobFileManage()
{
}

int JobFileManage::InitConfig(void)
{
	int nRet = 0;

	nRet = ReadConfigData(JOBCONFIG, strlen(JOBCONFIG));

	return nRet;
}

int JobFileManage::GetCurJobFileName(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (pBuffer != NULL)
	{
		sprintf_s(pBuffer, nLen, "%s/%02d_%s", JOBDIR, m_unJobIndex, JOBNAME);

		nRet = 1;
	}

	return nRet;
}

int JobFileManage::GetNextJobFileName(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (pBuffer != NULL)
	{
		m_unJobIndex++;

		if (SaveConfigData(JOBCONFIG, strlen(JOBCONFIG)))
		{
			sprintf_s(pBuffer, nLen, "%s/%02d_%s", JOBDIR, m_unJobIndex, JOBNAME);

			nRet = 1;
		}
	}

	return nRet;
}

int JobFileManage::ReadConfigData(char *pFileDir, int nLen)
{
	int nRet = 0;

	if (pFileDir != NULL)
	{
		FILE *pFile = NULL;
		char acConfig[256] = { 0 };

		fopen_s(&pFile, pFileDir, "rb");
		if (pFile != NULL)
		{
			fread(acConfig, sizeof(char), sizeof(acConfig), pFile);

			char *pTemp = strstr(acConfig, JOBINDEX);
			if (pTemp != NULL)
			{
				pTemp += (strlen(JOBINDEX) + 1);

				m_unJobIndex = atoi(pTemp);

				nRet = 1;
			}

			fclose(pFile);
		}
	}

	return nRet;
}

int JobFileManage::SaveConfigData(char *pFileDir, int nLen)
{
	int nRet = 0;

	if (pFileDir != NULL)
	{
		FILE *pFile = NULL;
		char acConfig[256] = { 0 };

		fopen_s(&pFile, pFileDir, "wb");
		if (pFile != NULL)
		{
			sprintf_s(acConfig, sizeof(acConfig), "<%s>%02d</%s>", JOBINDEX, m_unJobIndex, JOBINDEX);

			fwrite(acConfig, sizeof(char), strlen(acConfig), pFile);

			fclose(pFile);

			nRet = 1;
		}
	}

	return nRet;
}