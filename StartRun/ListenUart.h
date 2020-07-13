#ifndef LISTEN_UART_H_
#define LISTEN_UART_H_

#include "APLock/APLock.h"
#include "../Communication/Serial/Serial.h"
#include "ThreadRun/ThreadRun.h"
#include "TransportProtocol/TransportProtocol.h"
#include <list>
#include "ListenConnInterface.h"

using namespace std;

class ListenUart : public ThreadRun, public ListenConnInterface
{
public:
	ListenUart();
	~ListenUart();

public:
	void SetConnectInfo(char *pBuffer, int nLen);
	int ParseSTBCommand(void);
	int OpenUart(void);
	void CloseConn(void);
	int RetsetUart(void);
	void ClearData(void);

	int StartRun(char *pBuffer, int nLen);
	void Onceprocess(void);

	int ReadSTBCommand(int & nCommand);
	int ReadLog(char *pBuffer, int nLen);
	int SendDataTo(unsigned char *pBuffer, unsigned int unLen, unsigned int unCommand);
	int SendDataTo(unsigned int unCommand);
	int GetLastCommand(CommandData *pCommandData);
	int Reconnect(char *pBuffer, int nLen);

private:
	Serial m_Serial;
	APLock m_UartLock;
	TransportProtocol m_TransportProtocol;

	char m_acUartCom[16];
	unsigned char m_aucRecv[16 * 1024];
	unsigned int m_unRecvLen;

	CommandData m_LastCommand;
};

#endif/*LISTEN_UART_H_*/