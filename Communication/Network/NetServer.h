#ifndef NET_SERVER_H_
#define NET_SERVER_H_

#include "Network.h"
#include "../../StartRun/ThreadRun/ThreadRun.h"
#include "../../StartRun/APLock/APLock.h"


class NetServer : public Network, public ThreadRun
{
public:
	NetServer();
	~NetServer();

public:
	int StartServer(char *pIPStr, unsigned short usPort);

	void Onceprocess();

	int WaitClient(SOCKET & stSocket);

	void CloseConn(void);

private:
	SOCKET m_stServerSocket;
	SOCKET m_stClient;
	APLock m_APLock;
};

#endif/*NET_SERVER_H_*/