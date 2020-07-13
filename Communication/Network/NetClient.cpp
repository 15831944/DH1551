#include "NetClient.h"

NetClient::NetClient()
{
	m_stClientSocket = INVALID_SOCKET;
}

NetClient::~NetClient()
{
	Disconnect(m_stClientSocket);
}

int NetClient::StartClient(char *pIPStr, unsigned short usPort)
{
	int nFinish = 0;

	m_stClientSocket = InitSocket();

	if (m_stClientSocket != INVALID_SOCKET)
	{
		if (ConnectNetServer(pIPStr, usPort, m_stClientSocket))
		{
			nFinish = 1;
		}
	}

	return nFinish;
}

int NetClient::SendDataTo(unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;

	if (pBuffer != NULL && unLen > 0)
	{
		nRet = send(m_stClientSocket, (char *)pBuffer, unLen, 0);
	}

	return nRet;
}

int NetClient::RecvDataFrom(unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;

	if (pBuffer != NULL && unLen > 0)
	{
		nRet = recv(m_stClientSocket, (char *)pBuffer, unLen, 0);
	}

	return nRet;
}

void NetClient::CloseConn(void)
{
	Disconnect(m_stClientSocket);
}