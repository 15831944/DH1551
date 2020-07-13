#include <afxwin.h>
#include "DebugLog.h"

static HANDLE hMutex = NULL; 
static char g_cDatabuf[1024*48] = {0};
static char g_ctemp[1024*48] = {0};
static unsigned int g_unDataLen = 0;
static char g_logfile[128] = {0};
static char g_ReportFile[128] = {0};

void InitLogFile(void)
{
	time_t nowTime;

	memset(g_logfile, 0, sizeof(g_logfile));
	memset(g_ReportFile, 0, sizeof(g_ReportFile));
   
    nowTime = time(NULL);  
 
    struct tm *sysTime = localtime(&nowTime);
	sprintf(g_logfile, "%04d%02d%02d_%02d%02d%02d_Log.txt",
		sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday, sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);
	sprintf(g_ReportFile, "%04d%02d%02d_%02d%02d%02d_Report.txt",
		sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday, sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);
}

void LogInit()
{
    hMutex = CreateMutex( NULL,  FALSE,   NULL ); 
	PrintfLog("[LogInit %d]hMutex:0x%x Finish...\r\n", __LINE__, hMutex);

	InitLogFile();
	PrintfLog("[LogInit %d]InitLogFile Finish...\r\n", __LINE__);
}

void GetLogBuffer(char **p, unsigned int *pLen)
{
	MutexWait(INFINITE);
	
	*p = (char*)g_cDatabuf;
	*pLen = g_unDataLen;
	
	MutexRelease();
}

void WriteReportOneLineAT(char *line)
{
	FILE *pFOutLog = NULL;
	if(strlen(g_ReportFile)>0)
	{
		pFOutLog = fopen(g_ReportFile, "at");//追加打开一个文本文件，并在文件末尾写数据
		if(0 != pFOutLog)
		{
			fwrite(line, 1, strlen(line), pFOutLog);
			fclose(pFOutLog);
		}
	}
}

void WriteLogToFile(char *s)
{
	FILE *pFOutLog = NULL;
	if(strlen(g_logfile)>0)
	{
		pFOutLog = fopen(g_logfile, "at");//追加打开一个文本文件，并在文件末尾写数据
		if(0 != pFOutLog)
		{
			fwrite(s, 1, strlen(s), pFOutLog);
			fclose(pFOutLog);
		}
	}
}

void PrintfLog(char *s, ...)
{
	MutexWait(INFINITE);

	va_list list;

	memset(g_ctemp, 0, sizeof(g_ctemp));
	//g_ctemp[0] = '<';
	//g_ctemp[1] = 'P';
	//g_ctemp[2] = 'C';
	//g_ctemp[3] = '>';

	va_start(list, s);
	//vsprintf(&g_ctemp[4], s, list);
	vsprintf(g_ctemp, s, list);
	va_end(list);

	if (g_unDataLen + strlen(g_ctemp) >= sizeof(g_cDatabuf))
	{
		memset(g_cDatabuf, 0, sizeof(g_cDatabuf));
		g_unDataLen = 0;
	}
	memcpy(&g_cDatabuf[g_unDataLen], g_ctemp, strlen(g_ctemp));
	g_unDataLen += (strlen(g_ctemp));

	//WriteLogToFile(g_ctemp);

	MutexRelease();
}

void ClearLogBuffer()
{
	MutexWait(INFINITE);

	memset(g_cDatabuf, 0, sizeof(g_cDatabuf));
	g_unDataLen = 0;

	MutexRelease();
}

void PutUartChar(char C)
{
#if 1
	char str[2] = {0,0};
	MutexWait(INFINITE);
	
	g_cDatabuf[g_unDataLen] = C;
	g_unDataLen++;

	str[0] = C;
	//WriteLogToFile(str);

	MutexRelease();
#endif
}

int GetFileString(char *pFileName, int nLen, char *pBuffer, int nBufLen)
{
	int nRet = 0;

	if (pFileName != NULL && nLen > 0 && pBuffer != NULL && nBufLen > 0)
	{
		FILE *pFile = NULL;
		fopen_s(&pFile, pFileName, "rb");
		if (pFile != NULL)
		{
			nRet = fread(pBuffer, sizeof(char), nBufLen, pFile);

			fclose(pFile);
		}
	}

	return nRet;
}

int MutexWait(unsigned int unMillSec)
{
	int nRet = 0;

	if (hMutex != NULL)
	{
		nRet = WaitForSingleObject(hMutex, unMillSec);
	}

	return nRet;
}

int MutexRelease()
{
	int nRet = 0;

	if (hMutex != NULL)
	{
		nRet = ReleaseMutex(hMutex);
	}

	return nRet;
}