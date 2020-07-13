#ifndef _DEBUGLOG_34_H_
#define _DEBUGLOG_34_H_

void LogInit();

void PrintfLog(char *s, ...);

void GetLogBuffer(char **p, unsigned int *pLen);

void ClearLogBuffer();

void PutUartChar(char C);

void WriteReportOneLineAT(char *line);

int GetFileString(char *pFileName, int nLen, char *pBuffer, int nBufLen);

int MutexWait(unsigned int unMillSec);

int MutexRelease();

#endif

