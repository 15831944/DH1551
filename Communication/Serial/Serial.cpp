#include "Serial.h"

Serial::Serial(unsigned int unInputSize, unsigned int unOutputSize):
m_unInputSize(unInputSize), m_unOutputSize(unOutputSize)
{
	m_hCom = NULL;
}

Serial::~Serial()
{
	UartClose();
}

BOOL Serial::UartOpen(char acComName[], unsigned int unLength)
{
	DCB stDCB = { 0 };
	COMMTIMEOUTS stCommTimeOut;
	char acOpenCom[16] = { 0 };

	if (acComName == NULL || (m_hCom != NULL && m_hCom != INVALID_HANDLE_VALUE))
	{
		return FALSE;
	}

	if (unLength > 4)
	{
		sprintf_s(acOpenCom, sizeof(acOpenCom), "\\\\.\\%s", acComName);
	}
	else
	{
		sprintf_s(acOpenCom, sizeof(acOpenCom), "%s", acComName);
	}

	m_hCom = CreateFile(
		acOpenCom,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		NULL,
		NULL);

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	SetupComm(m_hCom, m_unInputSize, m_unOutputSize);

	PurgeComm(m_hCom, PURGE_RXABORT | PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	GetCommState(m_hCom, &stDCB);
	stDCB.BaudRate = 115200;
	stDCB.ByteSize = 8;
	stDCB.Parity = NOPARITY;
	stDCB.StopBits = ONESTOPBIT;
	stDCB.fAbortOnError = FALSE;// TRUE;

	if (!SetCommState(m_hCom, &stDCB))
	{
		UartClose();
		return FALSE;
	}

	GetCommTimeouts(m_hCom, &stCommTimeOut);

	stCommTimeOut.ReadIntervalTimeout = 0xFFFFFFFF;
	stCommTimeOut.ReadTotalTimeoutMultiplier = 0;
	stCommTimeOut.ReadTotalTimeoutConstant = 0;
	stCommTimeOut.WriteTotalTimeoutMultiplier = 0;
	stCommTimeOut.WriteTotalTimeoutConstant = 50000;	/* 5sec */

	if (!SetCommTimeouts(m_hCom, &stCommTimeOut))
	{
		UartClose();
		return FALSE;
	}

	return TRUE;
}

DWORD Serial::UartReceiveData(void *buffer, DWORD dwBytesToRead)
{
	DWORD dwErrorFlags = 0;
	COMSTAT ComStat = { 0 };
	DWORD dwDataLen = 0;
	DWORD dwFlag = 0;

	if (m_hCom == NULL)
	{
		return dwDataLen;
	}

	dwFlag = ClearCommError(m_hCom, &dwErrorFlags, &ComStat);//清除硬件通讯错误
	if (dwFlag == 0)
	{
		return -1;
	}

	if (ComStat.cbInQue > 0)
	{
		ReadFile(m_hCom, buffer, dwBytesToRead, &dwDataLen, NULL);
	}

	if (ComStat.cbInQue > m_unInputSize)
	{
		FILE *pFile = NULL;
		char acFileName[64] = { 0 };
		sprintf_s(acFileName, sizeof(acFileName), "%s_log.txt", "serial");
		fopen_s(&pFile, acFileName, "ab+");
		if (pFile != NULL)
		{
			fwrite(buffer, sizeof(unsigned char), dwDataLen, pFile);
			fclose(pFile);
		}
	}
	return dwDataLen;
}

DWORD Serial::UartSendData(const char *buffer, DWORD dwBytesToWrite)
{
	DWORD dwDataLen = 0;

	if (m_hCom == NULL)
	{
		return dwDataLen;
	}

	WriteFile(m_hCom, buffer, dwBytesToWrite, &dwDataLen, NULL);

	return dwDataLen;
}

BOOL Serial::UartReset(void)//清空缓存区
{
	return PurgeComm(m_hCom, PURGE_RXABORT | PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
}

BOOL Serial::UartClose(void)
{
	BOOL bRet = TRUE;

	m_APLock.WaitLock();

	if (m_hCom != NULL)
	{
		bRet = CloseHandle(m_hCom);
		m_hCom = NULL;
	}

	m_APLock.ReleaseLock();

	return bRet;
}

int Serial::StartConn(char *pBuffer, unsigned int unData)
{
	int nRet = 0;

	if (UartOpen(pBuffer, unData))
	{
		nRet = 1;
	}

	return nRet;
}

int Serial::SendDataToSTB(unsigned char *pBuffer, unsigned int unLen)
{
	return UartSendData((char*)pBuffer, unLen);
}

int Serial::RecvDataFromSTB(unsigned char *pBuffer, unsigned int unLen)
{
	return UartReceiveData((void*)pBuffer, unLen);
}

int Serial::CloseConn(void)
{
	int nRet = 0;

	if (UartClose())
	{
		nRet = 1;
	}

	return nRet;
}

int Serial::ResetConn(void)
{
	int nRet = 0;

	if (UartReset())
	{
		nRet = 1;
	}

	return nRet;
}

BOOL Serial::IsConnect(void)
{
	BOOL bRet = FALSE;

	if (m_hCom != NULL)
	{
		bRet = TRUE;
	}

	return bRet;
}