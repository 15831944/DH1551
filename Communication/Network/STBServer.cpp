#include "STBServer.h"

STBServer::STBServer()
{
	m_Lock = 0;

	m_nTimeOut = 0;

	m_hThread = NULL;
}

STBServer::~STBServer()
{
	if (m_hThread != NULL)
	{
		TerminateProcess(m_hThread, 0);
	}
}

int STBServer::StartConn(char *pBuffer, unsigned int unData)
{
	if (m_hThread == NULL)
	{
		StartThread();
	}

	return m_Network.StartServer(pBuffer, (unsigned short)unData);
}

int STBServer::IsGetConn(void)
{
	return m_Network.WaitClient();
}

int STBServer::SendDataToSTB(unsigned char *pBuffer, unsigned int unLen)
{
	return m_Network.SendDataTo(pBuffer, unLen);
}

int STBServer::RecvDataFromSTB(unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;

	SetLock();

	nRet = m_Network.RecvDataFrom(pBuffer, unLen);

	ReleaseLock();

	return nRet;
}

int STBServer::CloseConn(void)
{
	m_Network.CloseClient();

	return 1;
}

int STBServer::StartThread(void)
{
	DWORD dwThread;

	if (m_hThread == NULL)
	{
		m_hThread = CreateThread(NULL, 0, ThreadFun, this, 0, &dwThread);
	}

	SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);

	return 1;
}

DWORD WINAPI STBServer::ThreadFun(LPVOID lpThreadParameter)
{
	if (lpThreadParameter != NULL)
	{
		((STBServer*)lpThreadParameter)->CountTime();
	}

	return 1;
}

void STBServer::SetLock(void)
{
	m_nTimeOut = 0;

	m_Lock = 1;
}

void STBServer::ReleaseLock(void)
{
	m_Lock = 0;

	m_nTimeOut = 0;
}

void STBServer::CountTime(void)
{
	while (1)
	{
		if (m_Lock)
		{
			m_nTimeOut += 1000;
		}

		Sleep(1000);

		if (m_nTimeOut > 1000 && m_Lock)
		{
			CloseConn();
		}
	}
}
