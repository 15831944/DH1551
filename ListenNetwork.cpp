#include "ListenNetwork.h"

ListenNetwork::ListenNetwork()
{
	ClearData();

	memset(&m_LastCommand, 0, sizeof(CommandData));

	m_nConnCount = 0;
}

ListenNetwork::~ListenNetwork()
{
	Stop();
}

int ListenNetwork::StartRun(char *pIPStr, int nPort)
{
	return StartRun(pIPStr, (unsigned short)(nPort & 0xffff));
}

int ListenNetwork::StartRun(char *pIPStr, unsigned short usPort)
{
	int nRet = 0;

	if (m_NetServer.StartServer(pIPStr, usPort))
	{
		Start();

		nRet = 1;
	}

	return nRet;
}

void ListenNetwork::Onceprocess(void)
{
	if (m_NetServer.IsNetworkConnect())
	{
		SOCKET stSocket = INVALID_SOCKET;
		if (m_NetServer.WaitClient(stSocket))
		{
			m_nConnCount = 1;

			m_OneClient.SetSocket(stSocket);
		}

		if (m_OneClient.IsValidConnect())
		{
			int nActLen = m_OneClient.RecvDataFrom(&m_aucRecv[m_unRecvLen], sizeof(m_aucRecv)-m_unRecvLen);

			if (nActLen > 0)
			{
				m_unRecvLen += nActLen;

				ParseSTBCommand();
			}
			else if (nActLen < 0)
			{
				m_OneClient.StopConn();

				m_nConnCount = 0;
			}
			else if (nActLen == 0)
			{
				//m_OneClient.SendDataTo((unsigned char*)"00", 2);
			}
		}
	}

	Sleep(1000);
}

int ListenNetwork::ParseSTBCommand(void)
{
	int nRet = 0;
	int nIndex = 0;

	m_UartLock.WaitLock();

	for (int ii = 0; ii < m_unRecvLen; ii++)
	{
		nIndex = m_TransportProtocol.PraseData(&m_aucRecv[ii], m_unRecvLen - ii) + ii;
		if (nIndex >= ii)
		{
			memset(&m_LastCommand, 0, sizeof(CommandData));

			m_LastCommand.m_nCommand = m_aucRecv[nIndex + 1];

			m_LastCommand.m_nDataLen = (int)((m_aucRecv[nIndex + 2] << 8) | m_aucRecv[nIndex + 3]);

			memcpy(m_LastCommand.m_acData, &m_aucRecv[nIndex + 4], m_LastCommand.m_nDataLen);

			ii = nIndex + m_LastCommand.m_nDataLen;

			nRet = 1;
		}
	}

	m_UartLock.ReleaseLock();

	if (nRet == 1 || m_unRecvLen == sizeof(m_aucRecv))
	{
		ClearData();
	}

	return nRet;
}

void ListenNetwork::ClearData(void)
{
	memset(m_aucRecv, 0, sizeof(m_aucRecv));

	m_unRecvLen = 0;
}

int ListenNetwork::ReadLog(char *pBuffer, int nLen)
{
	int nRet = 0;
	FILE *pFile = NULL;
	static int nIndex = 0;
	int copylen = 0;

	char acFileName[64] = { 0 };

	m_UartLock.WaitLock();

	/*copylen = (nLen > m_unRecvLen - nIndex) ? (m_unRecvLen - nIndex) : nLen;
	if (nIndex < m_unRecvLen)
	{
		memcpy(pBuffer, &m_aucRecv[nIndex], nLen);

		nIndex = (copylen + nIndex) % sizeof(m_aucRecv);
	}*/

	m_UartLock.ReleaseLock();

	return nRet;
}

int ListenNetwork::RecvDataFrom(unsigned char *pBuffer, unsigned int unBufLen)
{
	return 0;
}

int ListenNetwork::SendDataTo(unsigned char *pBuffer, unsigned int unBufLen, unsigned int unCommand)
{
	int nRet = 0;
	int nSendLen = 0;
	int nActLen = 0;
	unsigned char *pTemp = NULL;

	pTemp = (unsigned char*)malloc(unBufLen + MIN_FACTORY_LEN);

	if (pTemp != NULL)
	{
		nSendLen = m_TransportProtocol.ConstructData(pBuffer, unBufLen, unCommand, pTemp, unBufLen + MIN_FACTORY_LEN);
		if (nSendLen > 0)
		{
			nActLen = m_OneClient.SendDataTo(pTemp, nSendLen);
			if (nSendLen == nActLen)
			{
				nRet = 1;
			}
		}
		free(pTemp);
	}

	return nRet;
}

int ListenNetwork::SendDataTo(unsigned int unCommand)
{
	return SendDataTo( NULL, 0, unCommand );
}

int ListenNetwork::GetLastCommand(CommandData *pCommandData)
{
	int nRet = 0;

	m_UartLock.WaitLock();

	if (m_LastCommand.m_nCommand != 0)
	{
		memcpy(pCommandData, &m_LastCommand, sizeof(CommandData));

		memset(&m_LastCommand, 0, sizeof(CommandData));

		nRet = 1;
	}

	m_UartLock.ReleaseLock();

	return nRet;
}

int ListenNetwork::Reconnect(char *pBuffer, int nLen)
{
	return 1;
}

void ListenNetwork::CloseConn(void)
{
	m_NetServer.CloseConn();
}