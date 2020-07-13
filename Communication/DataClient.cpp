#include "DataClient.h"

DataClient::DataClient()
{
}

DataClient::~DataClient()
{
}

int DataClient::StartConnectCastpalServer(char * pIPStr, unsigned short usPort)
{
	return m_NetClient.StartClient(pIPStr, usPort);
}

int DataClient::SendDataToCastpalServer(unsigned char *pBuffer, unsigned int unLen, unsigned int nDataType)
{
	int nSendLen = 0;
	DWORD dwCRC32;
	unsigned char aucSendData[1024] = { 0 };

	if (unLen > 1024)
	{
		return nSendLen;
	}

	aucSendData[0] = DATA_HEAD;
	aucSendData[1] = (nDataType >> 8) & 0xFF;
	aucSendData[2] = nDataType & 0xFF;
	aucSendData[3] = (unLen >> 8) & 0xFF;
	aucSendData[4] = unLen & 0xFF;

	if (pBuffer != NULL && unLen > 0)
	{
		memcpy(&aucSendData[5], pBuffer, unLen);
	}

	dwCRC32 = m_UpgradeCommon.CalculateCRC32(aucSendData, unLen + 5);
	aucSendData[unLen + 5] = (dwCRC32 >> 24) & 0xFF;
	aucSendData[unLen + 6] = (dwCRC32 >> 16) & 0xFF;
	aucSendData[unLen + 7] = (dwCRC32 >> 8) & 0xFF;
	aucSendData[unLen + 8] = (dwCRC32)& 0xFF;

	return m_NetClient.SendDataTo(aucSendData, unLen + MIN_DATA_SIZE);
}

int DataClient::RecvDataFromCastpalServer(unsigned char *pBuffer, unsigned int unLen, unsigned int & nDataType)
{
	int nReceLen = 0;
	int nDataLen = 0;
	unsigned char aucRecvData[1024] = { 0 };

	nReceLen = m_NetClient.RecvDataFrom(aucRecvData, sizeof(aucRecvData));
	if (nReceLen != INVALID_SOCKET && nReceLen > MIN_DATA_SIZE)
	{
		if (DATA_HEAD == aucRecvData[0] && m_UpgradeCommon.CheckCRC32(aucRecvData, nReceLen) > 0)
		{
			nDataType = ((aucRecvData[1] << 8) | aucRecvData[2]);
			nDataLen = ((aucRecvData[3] << 8) | aucRecvData[4]);
			if (nDataLen <= unLen)
			{
				memcpy(pBuffer, &aucRecvData[5], nDataLen);
			}
			else
			{
				nDataLen = 0;
			}
			m_UpgradeCommon.WriteLog("Receive Data Form Server Success,DataType=0x%x,nDataLen=%d\r\n", nDataType, nDataLen);
		}
	}

	return nDataLen;
}

void DataClient::CloseConn(void)
{
	m_NetClient.CloseConn();
}