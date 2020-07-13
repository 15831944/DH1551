#include "CastpalServerData.h"
#include <stdio.h>

CastpalServerData::CastpalServerData()
{
	memset(m_acIP, 0, sizeof(m_acIP));

	m_usPort = 0;

	m_enDataType = E_DATATYPE_MAX;
}

CastpalServerData::~CastpalServerData()
{
}

void CastpalServerData::SetIPAndPort(char *pIP, int nLen, unsigned short usPort)
{
	if (pIP != NULL && nLen < sizeof(m_acIP))
	{
		if (m_UpgradeCommon.IsIPValid(pIP, nLen))
		{
			memcpy(m_acIP, pIP, nLen);

			m_usPort = usPort;
		}
	}
}

unsigned int CastpalServerData::RequestDataFromServer(unsigned char *pBuffer, unsigned int unLen, DataType enDataType)
{
	unsigned int unRet = 0;

	do
	{
		if (enDataType == E_DATATYPE_REG || IsExistLocalData(enDataType) == 0)
		{
			if (strlen(m_acIP) == 0 || m_usPort == 0)
			{
				m_UpgradeCommon.WriteLog("**CastpalServer IP或Port错误**IP:%s**Port:%u**\n", m_acIP, m_usPort);
				break;
			}

			if (m_DataClient.StartConnectCastpalServer(m_acIP, m_usPort) <= 0)
			{
				m_UpgradeCommon.WriteLog("****StartConnect CastpalServer Error****\n");
				break;
			}

			if (m_DataClient.SendDataToCastpalServer(pBuffer, unLen, enDataType) <= 0)
			{
				m_UpgradeCommon.WriteLog("****SendData CastpalServer Error****\n");
				break;
			}

			if (RecvAndParseData(enDataType) == 0)
			{
				m_UpgradeCommon.WriteLog("****RecvData CastpalServer Error****\n");
				break;
			}			
		}

		unRet = 1;
	} while (0);

	m_DataClient.CloseConn();

	return unRet;
}

int CastpalServerData::RecvAndParseData(DataType enSendDataType)
{
	int nRet = 0;
	int nDataLen = 0;
	unsigned int unDataType = E_DATATYPE_MAX;
	unsigned char aucReceData[1024] = { 0 };

	nDataLen = m_DataClient.RecvDataFromCastpalServer(aucReceData, sizeof(aucReceData), unDataType);
	if (nDataLen > 0 && unDataType == enSendDataType)
	{
		m_UpgradeCommon.WriteLog("Receive Data Form Server Success,DataType=0x%x,nDataLen=%d\r\n", unDataType, nDataLen);
		if (unDataType == E_DATATYPE_REG)
		{
			nRet = 1;
		}
		else if (unDataType == m_enDataType)
		{
			if (ParseServerData(aucReceData, nDataLen) == 1)
			{
				nRet = 1;
			}
		}
	}
	else
	{
		m_UpgradeCommon.WriteLog("Receive Data Form Server Fail\r\n");
	}

	return nRet;
}

unsigned int CastpalServerData::ParseServerData(unsigned char *pBuffer, unsigned int unLen)
{
	unsigned int unRet = 0;
	unsigned int unIndex = 0;

	do
	{
		if (pBuffer == NULL || unLen == 0)
		{
			break;
		}

		if ((m_enDataType & E_DATATYPE_SN) == E_DATATYPE_SN)//为SN信息
		{
			unsigned int unSNIndex = 0;
			unsigned int unSNLen = 0;
			unSNIndex = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			unIndex += 2;
			if (unSNIndex == 0)
			{
				unIndex = 0;
				m_UpgradeCommon.WriteLog("ReceiveDataFormServer Success,But SN Index is 0!!!\r\n");
				break;
			}
			unSNLen = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			unIndex += 2;
			if (unSNLen > 0)
			{
				m_SN.SetSN((char*)&pBuffer[unIndex], unSNLen);
				unIndex += unSNLen;
			}
			else
			{
				unIndex = 0;
				m_UpgradeCommon.WriteLog("ReceiveDataFormServer Success,But SN Len is 0!!!\r\n");
				break;
			}
		}

		if ((m_enDataType & E_DATATYPE_MAC) == E_DATATYPE_MAC)
		{
			unsigned int unMACIndex = -1;
			unsigned int unMACLen = 0;
			unMACIndex = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			unIndex += 2;
			if (unMACIndex == -1)
			{
				unIndex = 0;
				m_UpgradeCommon.WriteLog("ReceiveDataFormServer Success,But MAC Index is Wrong!!!\r\n");
				break;
			}
			unMACLen = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			unIndex += 2;
			if (unMACLen > 0)
			{
				m_MAC.SetMac((char*)&pBuffer[unIndex], unMACLen);
				unIndex += unMACLen;
			}
			else
			{
				unIndex = 0;
				m_UpgradeCommon.WriteLog("ReceiveDataFormServer Success,But Mac Len is 0!!!\r\n");
				break;
			}
		}

		if ((m_enDataType & E_DATATYPE_HDCP) == E_DATATYPE_HDCP)
		{
			unsigned int unHDCPIndex = -1;
			unsigned int unHDCPLen = 0;
			unHDCPIndex = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			unIndex += 2;
			if (unHDCPIndex == -1)
			{
				unIndex = 0;
				m_UpgradeCommon.WriteLog("ReceiveDataFormServer Success,But HDCP Index is Wrong!!!\r\n");
				break;
			}
			unHDCPLen = (pBuffer[unIndex] << 8) | pBuffer[unIndex + 1];
			unIndex += 2;
			if (unHDCPLen > 0)
			{
				m_HDCP.SetHDCP(&pBuffer[unIndex], unHDCPLen);
				unIndex += unHDCPLen;
			}
			else
			{
				unIndex = 0;
				m_UpgradeCommon.WriteLog("ReceiveDataFormServer Success,But HDCP Len is 0!!!\r\n");
				break;
			}
		}

		unRet = 1;
	} while (0);

	return unRet;
}

void CastpalServerData::SaveUpgradeData(char *pFilename, unsigned char *pucBuffer, unsigned int unLen)
{
	FILE *pFILE = NULL;

	do
	{
		fopen_s(&pFILE, pFilename, "wb");
		if (pFILE == NULL)
		{
			m_UpgradeCommon.WriteLog("====open file fail!!!\r\n");
			break;
		}

		if (pucBuffer == NULL || unLen <= 0)
		{
			m_UpgradeCommon.WriteLog("=====升级数据为空!!!\r\n");
			break;
		}

		if (fwrite(pucBuffer, unLen, sizeof(char), pFILE) == 0)
		{
			m_UpgradeCommon.WriteLog("====write file fail!!!\r\n");
			break;
		}
	} while (0);

	if (pFILE != NULL)
	{
		fclose(pFILE);
	}
}

int CastpalServerData::ReadUpgradeFile(char *pFilename, unsigned char *pucBuffer, unsigned int unLen)
{
	int nRet = 0;

	FILE *pFILE = NULL;

	do
	{
		fopen_s(&pFILE, pFilename, "rb");
		if (pFILE == NULL)
		{
			m_UpgradeCommon.WriteLog("====open file <%s> fail!!!\r\n", pFilename);
			break;
		}

		if (pucBuffer == NULL || unLen <= 0)
		{
			m_UpgradeCommon.WriteLog("=====升级数据为空!!!\r\n");
			break;
		}

		if (fread(pucBuffer, sizeof(char), unLen, pFILE) == 0)
		{
			m_UpgradeCommon.WriteLog("====Read file fail!!!\r\n");
			break;
		}

		nRet = 1;
	} while (0);

	if (pFILE != NULL)
	{
		fclose(pFILE);
	}

	return nRet;
}

void CastpalServerData::InvalidUpgradeFile(void)
{
	m_SN.Clear();

	m_HDCP.Clear();

	m_MAC.Clear();
}

unsigned int CastpalServerData::IsExistLocalData(DataType enDataType)
{
	unsigned int unRet = 0;
	
	if (m_SN.GetUsedStatus() || m_HDCP.GetUsedStatus() || m_MAC.GetUsedStatus())
	{
		unRet = 1;
	}

	return unRet;
}

void CastpalServerData::SetDataType(DataType enDataType)
{
	m_enDataType = enDataType;
}

int CastpalServerData::GetSNLength(void)
{
	return m_SN.GetSNLength();
}

unsigned int CastpalServerData::GetSN(char *pBuffer, int nLen)
{
	return m_SN.GetSN(pBuffer, nLen);
}

unsigned int CastpalServerData::GetHDCP(unsigned char *pBuffer, unsigned int unLen)
{
	return m_HDCP.GetHDCPKey(pBuffer, unLen);
}

unsigned int CastpalServerData::GetMAC(char *pBuffer, int nLen)
{
	return m_MAC.GetMac(pBuffer, nLen);
}

void CastpalServerData::SetSNLength(int nLen)
{
	m_SN.SetSNLength(nLen);
}
