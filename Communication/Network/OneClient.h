#ifndef ONE_CLIENT_H_
#define ONE_CLIENT_H_

#include "Network.h"
#include "../STBConnInterface.h"

class OneClient : public Network
{
public:
	OneClient(SOCKET stSocket = INVALID_SOCKET);
	~OneClient();

public:
	int SendDataTo(unsigned char *pBuffer, unsigned int unBufLen);
	int RecvDataFrom(unsigned char *pBuffer, unsigned int unBufLen);
	void StopConn(void);
	void SetSocket(SOCKET stSocket);
	BOOL IsValidConnect(void);

private:
	SOCKET m_stClientSocket;
};

#endif/*ONE_CLIENT_H_*/