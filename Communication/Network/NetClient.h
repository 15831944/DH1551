#ifndef NET_CLIENT_H_
#define NET_CLIENT_H_

#include "Network.h"

class NetClient : public Network
{
public:
	NetClient();
	~NetClient();

public:
	int StartClient(char *pIPStr, unsigned short usPort);

	int SendDataTo(unsigned char *pBuffer, unsigned int unLen);

	int RecvDataFrom(unsigned char *pBuffer, unsigned int unLen);

	void CloseConn(void);

private:
	SOCKET m_stClientSocket;
};

#endif/*NET_CLIENT_H_*/