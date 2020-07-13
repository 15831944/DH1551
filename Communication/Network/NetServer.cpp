#include "NetServer.h"

NetServer::NetServer()
{
	m_stServerSocket = INVALID_SOCKET;

	m_stClient = INVALID_SOCKET;
}

NetServer::~NetServer()
{
	Stop();

	CloseConn();
}

int NetServer::StartServer(char *pIPStr, unsigned short usPort)
{
	int nFinish = 0;
	char acName[256] = { 0 };

	/*如果存在同名的套接口,则先删除*/
	_unlink("ServerSTBSocket");

	m_stServerSocket = InitSocket();

	if (m_stServerSocket != INVALID_SOCKET)
	{
		if (BindNetInfo(pIPStr, usPort, m_stServerSocket) != SOCKET_ERROR)
		{
			if (gethostname(acName, sizeof(acName)) == SOCKET_ERROR)
			{
				Disconnect(m_stServerSocket);
			}
			else
			{
				/*以上创建监听队列.等待用户的连接请求*/
				listen(m_stServerSocket, 10);

				Start();

				nFinish = 1;
			}
		}
	}

	return nFinish;
}

void NetServer::Onceprocess()
{
	SOCKET stSocket = INVALID_SOCKET;

	stSocket = accept(m_stServerSocket, NULL, NULL);

	if (stSocket != INVALID_SOCKET)
	{
		m_APLock.WaitLock();

		m_stClient = stSocket;

		m_APLock.ReleaseLock();
	}

	Sleep(100);
}

int NetServer::WaitClient(SOCKET & stSocket)
{
	int nRet = 0;

	m_APLock.WaitLock();

	if (m_stClient != INVALID_SOCKET)
	{
		stSocket = m_stClient;

		m_stClient = INVALID_SOCKET;

		nRet = 1;
	}

	m_APLock.ReleaseLock();

	return nRet;
}

void NetServer::CloseConn(void)
{
	Disconnect(m_stServerSocket);
}
