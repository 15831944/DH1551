#include "ListenUart.h"

ListenUart::ListenUart() :m_Serial(16 * 1024, 8 * 1024)
{
	ClearData();

	memset(&m_LastCommand, 0, sizeof(CommandData));

	memset(m_acUartCom, 0, sizeof(m_acUartCom));
}

ListenUart::~ListenUart()
{
	Stop();
}

void ListenUart::ClearData(void)
{
	memset(m_aucRecv, 0, sizeof(m_aucRecv));

	m_unRecvLen = 0;
}

int ListenUart::StartRun(char *pBuffer, int nLen)
{
	int nRet = 0;

	m_UartLock.WaitLock();

	if (m_Serial.StartConn(pBuffer, nLen))
	{
		m_Serial.ResetConn();

		nRet = 1;
	}
	SetConnectInfo(pBuffer, nLen);

	Start();

	m_UartLock.ReleaseLock();

	return nRet;
}

void ListenUart::Onceprocess(void)
{
	if (m_Serial.IsConnect())
	{
		int nActRecv = 0;

		nActRecv = m_Serial.RecvDataFromSTB(&m_aucRecv[m_unRecvLen], sizeof(m_aucRecv)-m_unRecvLen);

		if (nActRecv == -1)
		{
			m_Serial.CloseConn();
		}
		else
		{
			m_unRecvLen += nActRecv;

			ParseSTBCommand();			
		}
	}
	else
	{
		if (m_Serial.StartConn(m_acUartCom, strlen(m_acUartCom)))
		{
			m_Serial.ResetConn();

			ClearData();
		}
	}

	Sleep(500);
}

int ListenUart::ParseSTBCommand(void)
{
	int nRet = 0;
	int nIndex = 0;

	m_UartLock.WaitLock();

	for (int ii = 0; ii < m_unRecvLen; ii++)
	{
		nIndex = m_TransportProtocol.PraseData(&m_aucRecv[ii], m_unRecvLen-ii) + ii;
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
		FILE *pFile = NULL;
		char acFileName[64] = { 0 };
		sprintf_s(acFileName, sizeof(acFileName), "%s_print_log.txt", m_acUartCom);
		fopen_s(&pFile, acFileName, "ab+");
		if (pFile != NULL)
		{
			fwrite(m_aucRecv, sizeof(unsigned char), m_unRecvLen, pFile);
			fclose(pFile);
		}

		ClearData();
	}

	return nRet;
}

void ListenUart::SetConnectInfo(char *pBuffer, int nLen)
{
	if (pBuffer != NULL && nLen < sizeof(m_acUartCom))
	{
		memcpy(m_acUartCom, pBuffer, nLen);
	}
}

int ListenUart::ReadSTBCommand(int & nCommand)
{
	int nRet = 0;

	m_UartLock.WaitLock();
	if (m_LastCommand.m_nCommand)
	{
		nCommand = m_LastCommand.m_nCommand;

		nRet = 1;
	}
	m_UartLock.ReleaseLock();

	return nRet;
}

int ListenUart::ReadLog(char *pBuffer, int nLen)//show
{
	int nRet = 0;
	FILE *pFile = NULL;
	char acFileName[64] = { 0 };


	return nRet;
}

int ListenUart::GetLastCommand(CommandData *pCommandData)
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

int ListenUart::SendDataTo(unsigned char *pBuffer, unsigned int unLen, unsigned int unCommand)
{
	int nRet = 0;
	int nSendLen = 0;
	int nActLen = 0;
	unsigned char *pTemp = NULL;

	pTemp = (unsigned char*)malloc(unLen + MIN_FACTORY_LEN);

	if (pTemp != NULL)
	{
		nSendLen = m_TransportProtocol.ConstructData(pBuffer, unLen, unCommand, pTemp, unLen + MIN_FACTORY_LEN);
		if (nSendLen > 0)
		{
			nActLen = m_Serial.SendDataToSTB(pTemp, nSendLen);
			if (nSendLen == nActLen)
			{
				nRet = 1;
			}
		}
		free(pTemp);
	}

	return nRet;
}

int ListenUart::SendDataTo(unsigned int unCommand)
{
	return SendDataTo(NULL, 0, unCommand);
}

int ListenUart::Reconnect(char *pBuffer, int nLen)
{
	int nRet = 0;

	m_UartLock.WaitLock();

	m_Serial.CloseConn();

	if (m_Serial.StartConn(pBuffer, nLen))
	{
		SetConnectInfo(pBuffer, nLen);

		m_Serial.ResetConn();

		ClearData();

		nRet = 1;
	}

	m_UartLock.ReleaseLock();

	return nRet;
}

void ListenUart::CloseConn(void)
{
	m_Serial.CloseConn();
}