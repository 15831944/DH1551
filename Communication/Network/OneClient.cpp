#include "OneClient.h"

OneClient::OneClient(SOCKET stSocket) :m_stClientSocket(stSocket)
{
	if (stSocket != INVALID_SOCKET)
	{
		unsigned long ul = 1;

		ioctlsocket(m_stClientSocket, FIONBIO, &ul);
	}
}

OneClient::~OneClient()
{
	StopConn();
}

int OneClient::SendDataTo(unsigned char *pBuffer, unsigned int unBufLen)
{
	int nRet = 0;

	if (pBuffer != NULL && unBufLen > 0)
	{
		nRet = send(m_stClientSocket, (char *)pBuffer, unBufLen, 0);
	}

	return nRet;
}

int OneClient::RecvDataFrom(unsigned char *pBuffer, unsigned int unBufLen)
{
	int nRet = 0;
	int nError = 0;

	if (pBuffer != NULL && unBufLen > 0)
	{
		nRet = recv(m_stClientSocket, (char *)pBuffer, unBufLen, 0);

		if (nRet == SOCKET_ERROR)
		{
			nError = WSAGetLastError();
			if (WSAEWOULDBLOCK == nError)
			{
				nRet = 0;
			}
		}
	}

	return nRet;
}

void OneClient::StopConn(void)
{
	Disconnect(m_stClientSocket);
}

void OneClient::SetSocket(SOCKET stSocket)
{
	if (stSocket != INVALID_SOCKET)
	{
		unsigned long ul = 1;

		if (m_stClientSocket != INVALID_SOCKET)
		{
			Disconnect(m_stClientSocket);
		}

		m_stClientSocket = stSocket;

		ioctlsocket(m_stClientSocket, FIONBIO, &ul);
	}
}

BOOL OneClient::IsValidConnect(void)
{
	BOOL bRe = FALSE;

	if (m_stClientSocket != INVALID_SOCKET)
	{
		bRe = TRUE;
	}

	return bRe;
}