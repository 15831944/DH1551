#include "DataManage.h"
#include "../StartRun/UpgradeMsg.h"

DataManage::DataManage()
{
	m_nDataType = 0;
	m_ReqType = E_NULL_REQ;

	memset(m_acOrder, 0, sizeof(m_acOrder));
	memset(m_acBarCode, 0, sizeof(m_acBarCode));
}

DataManage::~DataManage()
{
}

unsigned int DataManage::RequestDataFromCastpalServer()
{
	return m_CastpalServerData.RequestDataFromServer((unsigned char*)m_acOrder, strlen(m_acOrder), (DataType)m_nDataType);
}

unsigned int DataManage::RegisterCastpalClient()
{
	return m_CastpalServerData.RequestDataFromServer((unsigned char*)m_acOrder, strlen(m_acOrder), E_DATATYPE_REG);
}

unsigned int DataManage::CheckWorkPoint(char *pLotSN, int nLotSNLen)
{
	return m_FactoryDBServerData.CheckWorkPoint(pLotSN, nLotSNLen);
}

unsigned int DataManage::InsertDataIntoDB(void)
{
	return m_FactoryDBServerData.InsertDataIntoDB();
}

unsigned int DataManage::CheckOrderInfo(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName)
{
	m_FactoryDBServerData.SetBaseFiledData(OrderID, TestDevice, WorkcenterName, UserName);

	return m_FactoryDBServerData.CheckOrderInfo();
}

unsigned int DataManage::GetSN(char *pBuffer, int nLen)
{
	return m_CastpalServerData.GetSN(pBuffer, nLen);
}

int DataManage::GetSNLength(void)
{
	return m_CastpalServerData.GetSNLength();
}

unsigned int DataManage::GetHDCP(unsigned char *pBuffer, unsigned int unLen)
{
	return m_CastpalServerData.GetHDCP(pBuffer, unLen);
}

unsigned int DataManage::GetMAC(char *pBuffer, int nLen)
{
	return m_CastpalServerData.GetMAC(pBuffer, nLen);
}

unsigned int DataManage::GetPEF(unsigned char *pChipID, unsigned char *pBuffer, unsigned int unLen)
{
	return m_PEF.ParseAllData(pChipID, pBuffer, unLen, E_PEF);
}

unsigned int DataManage::GetIFCP(unsigned char *pChipID, unsigned char *pBuffer, unsigned int unLen)
{
	return 0;//m_PEF.ParseAllData(pChipID, pBuffer, unLen, E_IFCP);
}

unsigned int DataManage::ParseReturnData(unsigned char *pBuffer, unsigned int unLen)
{
	return m_FactoryDBServerData.ParseReturnData(pBuffer, unLen);
}

unsigned int DataManage::ConstructPrintFile(void)
{
	return m_FactoryDBServerData.ConstructPrintFile();
}

void DataManage::SetBarCode(char *pBuffer, int nLen)
{
	if (pBuffer != NULL && nLen < sizeof(m_acBarCode))
	{
		memset(m_acBarCode, 0, sizeof(m_acBarCode));

		memcpy(m_acBarCode, pBuffer, nLen);
	}
}

unsigned int DataManage::GetBarCode(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL && nLen > strlen(m_acBarCode))
	{
		memcpy(pBuffer, m_acBarCode, strlen(m_acBarCode));

		unRet = strlen(m_acBarCode);
	}

	return unRet;
}

void DataManage::ClearAll(void)
{
	m_CastpalServerData.InvalidUpgradeFile();
}

unsigned int DataManage::VerifyBarCode(unsigned char *pBuffer, int nLen)
{
	return m_FactoryDBServerData.VerifyBarCode(pBuffer, nLen, m_acBarCode, strlen(m_acBarCode));
}

unsigned int DataManage::GetReqType(void)
{
	return m_ReqType;
}

unsigned int DataManage::InitPersonalize(void)
{
	return m_ConaxPersonalize.InitPersonalize();
}

unsigned int DataManage::LoadJobFile(void)
{
	return m_ConaxPersonalize.LoadJobfile();
}

unsigned int DataManage::RequestEncryptKey(unsigned char *pBuffer, unsigned int *punLen, unsigned int unChipID)
{
	return m_ConaxPersonalize.RequestEncrytionKey(unChipID, pBuffer, punLen);
}
unsigned int DataManage::RequestVscData(unsigned char *pBuffer, unsigned int *punLen)
{
	return m_ConaxPersonalize.RequestVSCData(pBuffer, punLen);
}

unsigned int DataManage::GetBaseRequestData(unsigned char *pBuffer, unsigned int & unLen, unsigned char *pChipid, unsigned int unChipLen)
{
	unsigned int unRet = 0;
	unsigned int unIndex = 0;

	do
	{
		if (m_ReqType & E_SN_REQ)
		{
			char acSN[32] = { 0 };
			int nSNLen = 0;

			nSNLen = GetSN(acSN, sizeof(acSN));
			if (nSNLen <= 0)
			{
				unRet = E_UPGRADE_GET_SN_ERROR;
				break;
			}
			pBuffer[unIndex] = (nSNLen >> 8) & 0xff;
			pBuffer[unIndex + 1] = nSNLen & 0xff;
			unIndex += 2;
			memcpy(&pBuffer[unIndex], acSN, nSNLen);
			unIndex += nSNLen;
		}

		if (m_ReqType & E_HDCP_REQ)
		{
			unsigned char aucHDCP[520] = { 0 };
			unsigned int unHDCPLen = 0;

			unHDCPLen = GetHDCP(aucHDCP, sizeof(aucHDCP));
			if (unHDCPLen <= 0)
			{
				unRet = E_UPGRADE_GET_HDCPKEY_ERROR;
				break;
			}
			pBuffer[unIndex] = (unHDCPLen >> 8) & 0xff;
			pBuffer[unIndex + 1] = unHDCPLen & 0xff;
			unIndex += 2;
			memcpy(&pBuffer[unIndex], aucHDCP, unHDCPLen);
			unIndex += unHDCPLen;
		}

		if (m_ReqType & E_MAC_REQ)
		{
			char acMac[32] = { 0 };
			int nMacLen = 0;

			nMacLen = GetMAC(acMac, sizeof(acMac));
			if (nMacLen <= 0)
			{
				unRet = E_UPGRADE_GET_MAC_ERROR;
				break;
			}
			pBuffer[unIndex] = (nMacLen >> 8) & 0xff;
			pBuffer[unIndex + 1] = nMacLen & 0xff;
			unIndex += 2;
			memcpy(&pBuffer[unIndex], acMac, nMacLen);
			unIndex += nMacLen;
		}

		if (m_ReqType & E_PEF_REQ)
		{
			unsigned char aucPEFData[16 * 1024] = { 0 };
			unsigned int unPEFLen = 0;

			unPEFLen = GetPEF(pChipid, aucPEFData, sizeof(aucPEFData));
			if (unPEFLen <= 0)
			{
				unRet = E_UPGRADE_GET_PEF_ERROR;
				break;
			}
			pBuffer[unIndex] = (unPEFLen >> 8) & 0xff;
			pBuffer[unIndex + 1] = unPEFLen & 0xff;
			unIndex += 2;
			memcpy(&pBuffer[unIndex], aucPEFData, unPEFLen);
			unIndex += unPEFLen;
		}

		unRet = 1;

		unLen = unIndex;
	} while (0);

	return unRet;
}

unsigned int DataManage::GetDBSTBType(char *pBuffer, int nLen)
{
	return m_FactoryDBServerData.GetDBSTBType(pBuffer, nLen);
}

unsigned int DataManage::GetDBDataType(void)
{
	return  m_FactoryDBServerData.GetDBDataType();
}

void DataManage::SetConfigData(ConfigData csConfigData)
{
	unsigned int unReturnType = E_CHIP_RT;

	memcpy(&m_ConfigData, &csConfigData, sizeof(m_ConfigData));

	m_CastpalServerData.SetSNLength(csConfigData.m_nSNLen);

	if (csConfigData.m_nSN)
	{
		m_nDataType |= E_DATATYPE_SN;

		unReturnType |= E_SN_RT;

		m_ReqType |= E_SN_REQ;
	}

	if (csConfigData.m_nHDCP)
	{
		m_nDataType |= E_DATATYPE_HDCP;

		m_ReqType |= E_HDCP_REQ;
	}

	if (csConfigData.m_nMAC)
	{
		m_nDataType |= E_DATATYPE_MAC;

		unReturnType |= E_MAC_RT;

		m_ReqType |= E_MAC_REQ;
	}

	if (csConfigData.m_nPEF)
	{
		m_ReqType |= E_PEF_REQ;
	}

	if (csConfigData.m_nPersonal)
	{
		unReturnType |= E_JOBID_RT;
		unReturnType |= E_VSCID_RT;
		unReturnType |= E_VALID_RT;
	}

	csConfigData.m_unDBDataType = unReturnType;

	memcpy(m_acOrder, csConfigData.m_acOrderNum, strlen(csConfigData.m_acOrderNum));

	m_CastpalServerData.SetDataType((DataType)m_nDataType);

	m_CastpalServerData.SetIPAndPort(csConfigData.m_acServerIP, strlen(csConfigData.m_acServerIP), csConfigData.m_ServerPort);

	m_FactoryDBServerData.SetReturnFileType(csConfigData.m_unDBDataType, csConfigData.m_acSTBType);
	m_FactoryDBServerData.SetPlatForm(csConfigData.m_acPlatID, strlen(csConfigData.m_acPlatID));

	//m_FactoryDBServerData.SetBaseFiledData(csConfigData.m_acOrderNum, csConfigData.m_acStep, csConfigData.m_acLine, csConfigData.m_acPersonID);

	m_ConaxPersonalize.SetNetworkInfo(csConfigData.m_acPersonalIP, strlen(csConfigData.m_acPersonalIP), csConfigData.m_PersonalPort);
}
