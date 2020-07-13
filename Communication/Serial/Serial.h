#ifndef SERIAL_H_
#define SERIAL_H_

#include <afxwin.h>
#include "../STBConnInterface.h"
#include "../../StartRun/APLock/APLock.h"

class Serial : public STBConnInterface
{
public:
	Serial(unsigned int unInputSize, unsigned int unOutputSize);
	~Serial();

public:
	int StartConn(char *pBuffer, unsigned int unData);
	int SendDataToSTB(unsigned char *pBuffer, unsigned int unLen);
	int RecvDataFromSTB(unsigned char *pBuffer, unsigned int unLen);
	int CloseConn(void);
	int ResetConn(void);
	BOOL IsConnect(void);

private:
	BOOL UartOpen(char acComName[], unsigned int unLength);
	DWORD UartReceiveData(void *buffer, DWORD dwBytesToRead);
	DWORD UartSendData(const char *buffer, DWORD dwBytesToWrite);
	BOOL UartReset(void);
	BOOL UartClose(void);

private:
	HANDLE m_hCom;
	APLock m_APLock;

	unsigned int m_unInputSize;
	unsigned int m_unOutputSize;
};

#endif/*SERIAL_H_*/