#ifndef LISTEN_NETWORK_H_
#define LISTEN_NETWORK_H_

#include "APLock/APLock.h"
#include "../Communication/Network/NetServer.h"
#include "../Communication/Network/OneClient.h"
#include "ThreadRun/ThreadRun.h"
#include "TransportProtocol/TransportProtocol.h"
#include "ListenConnInterface.h"

class ListenNetwork : public ThreadRun, public ListenConnInterface
{
public:
	ListenNetwork();
	~ListenNetwork();

public:
	int StartRun(char *pIPStr, int nPort);
	int StartRun(char *pIPStr, unsigned short usPort);
	void Onceprocess(void);
	int ParseSTBCommand(void);
	void ClearData(void);
	void CloseConn(void);

	int ReadLog(char *pBuffer, int nLen);
	int RecvDataFrom(unsigned char *pBuffer, unsigned int unBufLen);
	int SendDataTo(unsigned char *pBuffer, unsigned int unBufLen, unsigned int unCommand);
	int SendDataTo(unsigned int unCommand);
	int GetLastCommand(CommandData *pCommandData);
	int Reconnect(char *pBuffer, int nLen);

private:
	NetServer m_NetServer;
	OneClient m_OneClient;
	int m_nConnCount;

	APLock m_UartLock;
	TransportProtocol m_TransportProtocol;

	unsigned char m_aucRecv[16 * 1024];
	unsigned int m_unRecvLen;
	CommandData m_LastCommand;
};

#endif/*LISTEN_NETWORK_H_*/