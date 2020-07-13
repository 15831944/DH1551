#include "UpgradeSTB.h"

extern int g_DisconnectMesFlag;

UpgradeSTB::UpgradeSTB()
{
	m_enLedStatus.m_enStatus = E_Led_End;
	m_enLedStatus.m_nCheck = 1;
	m_unLastCommand = 0;
	m_unCurCommand = 1;
	m_unErrorCommand = 0;
	m_bEnableVerify = FALSE;
}

UpgradeSTB::~UpgradeSTB()
{
	Stop();

	if (m_pListenConnInterface != NULL)
	{
		delete m_pListenConnInterface;
	}
}

void UpgradeSTB::CloseConn(void)
{
	if (m_pListenConnInterface != NULL)
	{
		m_pListenConnInterface->CloseConn();
	}
}

int UpgradeSTB::Init(char *pBuffer, int nLen, int nType)
{
	int nRet = 0;

	if (m_pListenConnInterface == NULL)
	{
		if (nType == 0)
		{
			m_pListenConnInterface = new ListenUart();
		}
		else
		{
			m_pListenConnInterface = new ListenNetwork();
		}
	}

	if (m_pListenConnInterface != NULL)
	{
		if (m_pListenConnInterface->StartRun(pBuffer, nLen))
		{
			nRet = 1;

			m_unErrorCommand = ((6 << 16) | 4);
		}
		else
		{
			m_unErrorCommand = ((6 << 16) | 3);
		}

		Start();
	}

	return nRet;
}

void UpgradeSTB::SetConfig(ConfigData csConfigData)
{
	m_DataManage.SetConfigData(csConfigData);
}

int UpgradeSTB::ReadLog(char *pBuffer, int nLen)
{
	int nRet = 0;

	m_StatusLock.WaitLock();

	if (m_unErrorCommand != 0)
	{
		m_CommunicateMessageShow.ShowErrorMessage(m_unErrorCommand, pBuffer, nLen);
		m_unLastCommand = m_unCurCommand;
		m_unCurCommand = 0;
		m_unErrorCommand = 0;
		nRet = 1;
	}
	else if (m_unLastCommand != m_unCurCommand)
	{
		m_CommunicateMessageShow.ShowSTBMessage(m_unCurCommand, pBuffer, nLen);
		m_unLastCommand = m_unCurCommand;
		nRet = 1;
	}
	m_StatusLock.ReleaseLock();

	return nRet;
}

int UpgradeSTB::GetLastCommand(CommandData *pCommandData)
{
	int nRet = 0;
	static int nCount = 0;

	if (m_pListenConnInterface->GetLastCommand(pCommandData))
	{
		if (m_unCurCommand != pCommandData->m_nCommand || nCount > 3 || pCommandData->m_nCommand == E_UPGRADE_COMMAND_STB_REQ_IFCP)//E_UPGRADE_COMMAND_STB_SEND_WAIT_VERIFY)
		{
			m_unLastCommand = m_unCurCommand;
			m_unCurCommand = pCommandData->m_nCommand;
			nRet = 1;
			nCount = 0;
		}
		else
		{
			nCount++;
		}
	}

	return nRet;
}

void UpgradeSTB::Onceprocess()
{
	m_StatusLock.WaitLock();

	CommandData csCommandData;
	memset(&csCommandData, 0, sizeof(CommandData));
#if 0 
	csCommandData.m_acData[0] = 0xe0;
	csCommandData.m_acData[1] = 0x10;
	csCommandData.m_acData[2] = 0x57;
	csCommandData.m_acData[3] = 0x4e;
	csCommandData.m_nDataLen = 4;
	RequestIFCPSize(E_UPGRADE_COMMAND_PC_SEND_IFCP_SIZE, csCommandData.m_acData, csCommandData.m_nDataLen);

#endif
	if (GetLastCommand(&csCommandData))
	{
		if (m_enLedStatus.m_enStatus != E_Led_Start && m_enLedStatus.m_nCheck != 0)
		{
			m_enLedStatus.m_enStatus = E_Led_Start;
			m_enLedStatus.m_nCheck = 0;
		}

		switch (csCommandData.m_nCommand)
		{
		case E_UPGRADE_COMMAND_STB_SEND_UPGRADE_COMMAND:
		{
			RequestServerData(E_UPGRADE_COMMAND_PC_SEND_ACK);
		}
			break;

		case E_UPGRADE_COMMAND_STB_REQ_SN:
			SendServerData(E_UPGRADE_COMMAND_PC_SEND_SN, csCommandData.m_acData, csCommandData.m_nDataLen);
			break;

		case E_UPGRADE_COMMAND_STB_REQ_HDCP:
			break;

		case E_UPGRADE_COMMAND_STB_REQ_MAC:
			break;

		case E_UPGRADE_COMMAND_STB_REQ_PEF:
			break;

		case E_UPGRADE_COMMAND_STB_SEND_WAIT_SEND_UPGRADE_RESULT:
		{
			RequestReturnFile(E_UPGRADE_COMMAND_PC_SEND_REQ_UPGRADE_RESULT);
		}
			break;

		case E_UPGRADE_COMMAND_STB_SEND_UPGRADE_RESULT:
		{
			SendResult(E_UPGRADE_COMMAND_PC_SEND_UPGRADE_RESULT, csCommandData.m_acData, csCommandData.m_nDataLen);
		}
			break;

		case E_UPGRADE_COMMAND_STB_SEND_INSERT_DB:
		{
			InsertDataIntoDB(E_UPGRADE_COMMAND_PC_SEND_INSERT_RESULT);
		}
			break;

		case E_UPGRADE_COMMAND_STB_SEND_WAIT_VERIFY:
		{
			RequestVerifyData(E_UPGRADE_COMMAND_PC_SEND_VERIFY_COMMNAD);
		}
			break;

		case E_UPGRADE_COMMAND_STB_SEND_VERIFY_DATA:
		{
			StartVerify(E_UPGRADE_COMMAND_PC_SEND_VERIFY_RESULT, csCommandData.m_acData, csCommandData.m_nDataLen);
		}
			break;

		case E_UPGRADE_COMMAND_STB_SEND_VERIFY_FINISHED:
			m_enLedStatus.m_enStatus = E_Led_End;
			m_enLedStatus.m_nCheck = 0;
			break;

		case E_UPGRADE_COMMAND_STB_SEND_ERASE_RESULT:
			m_unErrorCommand = (5 << 16) | csCommandData.m_acData[0];
			break;

		case E_UPGRADE_COMMAND_STB_SEND_CHIPID_ERROR:
			m_unErrorCommand = (1 << 16) | E_UPGRADE_CHIPID_ERROR;
			break;

		case E_UPGRADE_COMMAND_STB_SEND_UPGRADE_STATUS:
			break;

		case E_UPGRADE_COMMAND_STB_SEND_PERSONALIZE_COMMAND:
		{
			RequestEncryptKey(E_UPGRADE_COMMAND_PC_SEND_ENCRYT_KEY, csCommandData.m_acData, csCommandData.m_nDataLen);
		}
			break;

		case E_UPGRADE_COMMAND_STB_SEND_ENCRYT_RESULT:
		{
			SendVSCDataLen(E_UPGRADE_COMMAND_PC_SEND_VSCDATA_LENGTH, csCommandData.m_acData, csCommandData.m_nDataLen);
		}
			break;

		case E_UPGRADE_COMMAND_STB_REQUEST_VSCDATA:
		{
			SendVSCData(E_UPGRADE_COMMAND_PC_SEND_VSCDATA, csCommandData.m_acData, csCommandData.m_nDataLen);
		}
			break;

		case E_UPGRADE_COMMAND_STB_RECV_VSCDATA:
		{
			RequestPersonalizeResult(E_UPGRADE_COMMAND_PC_REQ_PERSONALIZE_RESULT, csCommandData.m_acData, csCommandData.m_nDataLen);
		}
			break;
		
		case E_UPGRADE_COMMAND_STB_SEND_PERSONALIZE_RESULT:
		if (csCommandData.m_acData[0] == 0)
		{
			m_unErrorCommand = (7 << 16) | E_PERSONALIZE_FAIL;
		}
			break;

		case E_UPGRADE_COMMAND_STB_SEND_PHY_INTERFACE_RESULT:
		{
			m_pListenConnInterface->SendDataTo(E_UPGRADE_COMMAND_PC_RES_PHY_INTERFACE_RESULT);
		}
			break;

		case E_UPGRADE_COMMAND_STB_REQ_IFCP_SIZE:
		{
			RequestIFCPSize(E_UPGRADE_COMMAND_PC_SEND_IFCP_SIZE, csCommandData.m_acData, csCommandData.m_nDataLen);
		}
			break;

		case E_UPGRADE_COMMAND_STB_SEND_IFCP_RESULT:
		{
			SendIFCPResult(E_UPGRADE_COMMAND_PC_SEND_IFCP_RESULT);
		}
			break;

		/*case E_UPGRADE_COMMAND_STB_REQ_IFCP:
		{
			RequestIFCPData(E_UPGRADE_COMMAND_PC_SEND_IFCP, csCommandData.m_acData, csCommandData.m_nDataLen);
		}
			break;*/

		default:
		{
				   if (csCommandData.m_nCommand >= E_UPGRADE_COMMAND_STB_REQ_IFCP && csCommandData.m_nCommand <= E_UPGRADE_COMMAND_STB_REQ_IFCP_MAX)
				   {
					   for (unsigned int ii = E_UPGRADE_COMMAND_STB_REQ_IFCP; ii < E_UPGRADE_COMMAND_STB_REQ_IFCP_MAX; ii++)
					   {
						   if (ii == csCommandData.m_nCommand)
						   {
							   RequestIFCPData(ii+1, csCommandData.m_acData, csCommandData.m_nDataLen);
							   break;
						   }
					   }
				   }
		}
			break;
		}
	}

	m_StatusLock.ReleaseLock();

	Sleep(500);
}

int UpgradeSTB::RequestServerData(unsigned int unCommand)
{
	int nRet = 0;

	if (m_DataManage.RequestDataFromCastpalServer())
	{
		if (m_pListenConnInterface->SendDataTo(unCommand))
		{
			nRet = 1;
		}
	}

	if (nRet != 1)
	{
		m_unErrorCommand = (1 << 16) | E_UPGRADE_REQUEST_DATA_FAIL;
	}

	return nRet;
}

int UpgradeSTB::SendServerData(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;
	unsigned char aucStr[16 * 1024] = { 0 };
	unsigned int unStrLen = sizeof(aucStr);
	unsigned int unError = m_DataManage.GetBaseRequestData(aucStr, unStrLen, pBuffer, unLen);

	if (unError == 1)
	{
		if (m_pListenConnInterface->SendDataTo(aucStr, unStrLen, unCommand))
		{
			nRet = 1;
		}
	}

	if (nRet != 1)
	{
		m_unErrorCommand = (1 << 16) | unError;
	}

	return nRet;
}

int UpgradeSTB::RequestReturnFile(unsigned int unCommand)
{
	int nRet = 0;

	if (m_pListenConnInterface->SendDataTo(unCommand))
	{
		m_DataManage.ClearAll();
		nRet = 1;
	}

	return nRet;
}

int UpgradeSTB::SendResult(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;
	unsigned char aucStr[8] = { 0 };

	if (pBuffer[0] == 1)
	{
		if (m_DataManage.ParseReturnData(&pBuffer[1], unLen))
		{
			aucStr[0] = 1;
			if (m_pListenConnInterface->SendDataTo(aucStr, 1, unCommand))
			{
				nRet = 1;
			}
		}
	}

	if (nRet != 1)
	{
		m_unErrorCommand = (2 << 16);
	}

	return nRet;
}

unsigned int UpgradeSTB::CheckWorkPoint(char *pLotSN, int nLotSNLen)
{
	return m_DataManage.CheckWorkPoint(pLotSN, nLotSNLen);
}

int UpgradeSTB::InsertDataIntoDB(unsigned int unCommand)
{
	int nRet = 0;
	unsigned char aucStr[8] = { 0 };

	if (m_DataManage.InsertDataIntoDB())
	{
		aucStr[0] = 1;
		if (m_pListenConnInterface->SendDataTo(aucStr, 1, unCommand))
		{
			if (m_DataManage.ConstructPrintFile())
			{
				ShellExecuteA(NULL, "open", "D:\\BarCode\\barprint.bat", NULL, NULL, SW_HIDE);
			}
			else
			{
				m_unErrorCommand = (3 << 16) | 4;
			}

			nRet = 1;
		}
	}

	if (nRet != 1)
	{
		m_unErrorCommand = (3 << 16);
	}

	return nRet;
}

unsigned int UpgradeSTB::CheckOrderInfo(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName)
{
	return m_DataManage.CheckOrderInfo(OrderID, TestDevice, WorkcenterName, UserName);
}

void UpgradeSTB::SetConfigData(ConfigData csConfigData)
{
	m_DataManage.SetConfigData(csConfigData);

	m_CommunicateMessageShow.SetConnType(csConfigData.m_nConnType);
}

unsigned int UpgradeSTB::GetDBSTBType(char *pBuffer, int nLen)
{
	return m_DataManage.GetDBSTBType(pBuffer, nLen);
}

unsigned int UpgradeSTB::GetDBDataType(void)
{
	return m_DataManage.GetDBDataType();
}

int UpgradeSTB::SetBarCode(char *pBuffer, int nLen)
{
	int nRe = 0;

	if (pBuffer != NULL && nLen > 0)
	{
		m_DataManage.SetBarCode(pBuffer, nLen);
		m_bEnableVerify = TRUE;
		nRe = 1;
	}

	return nRe;
}

int UpgradeSTB::RequestVerifyData(unsigned int unCommand)
{
	int nRet = 0;

	if (m_bEnableVerify)
	{
		if (m_pListenConnInterface->SendDataTo(unCommand))
		{
			nRet = 1;
		}
	}

	return nRet;
}

int UpgradeSTB::StartVerify(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;
	unsigned char aucStr[8] = { 0 };

	if (pBuffer[0] == 1)
	{
		if (m_DataManage.VerifyBarCode(&pBuffer[1], unLen))
		{
			aucStr[0] = 1;
			if (m_pListenConnInterface->SendDataTo(aucStr, 1, unCommand))
			{
				nRet = 1;
			}
			if (g_DisconnectMesFlag == 0)
			{
				if (m_DataManage.InsertDataIntoDB() != 1)
				{
					m_unErrorCommand = (3 << 16);
					m_bEnableVerify = FALSE;
					return nRet;
				}
				//多发送一次
				m_pListenConnInterface->SendDataTo(aucStr, 1, unCommand);
			}
		}
		else
		{
			m_unErrorCommand = (9 << 16) | 0;
		}
	}
	else
	{
		m_unErrorCommand = (9 << 16) | 6;
	}
	m_bEnableVerify = FALSE;

	return nRet;
}

int UpgradeSTB::StartEraseAllData(void)
{
	int nRet = 0;

	if (m_pListenConnInterface->SendDataTo(E_UPGRADE_COMMAND_PC_SEND_ERASE_COMMNAD))
	{
		nRet = 1;
	}

	return nRet;
}

int UpgradeSTB::StartRuqestReturnFile(void)
{
	int nRet = 0;

	if (m_pListenConnInterface->SendDataTo(E_UPGRADE_COMMAND_PC_SEND_REQ_RETURNFILE_AGAIN))
	{
		nRet = 1;
	}

	return nRet;
}

int UpgradeSTB::QuerySTBStatus(void)
{
	int nRet = 0;

	if (m_pListenConnInterface->SendDataTo(E_UPGRADE_COMMAND_PC_REQ_UPGRADE_STATUS))
	{
		nRet = 1;
	}

	return nRet;
}

int UpgradeSTB::RequestEncryptKey(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;
	unsigned int unChipID = 0;
	unsigned char aucEncryptKey[128] = { 0 };
	unsigned int unEncryptKeyLen = 0;

	unChipID = (unsigned int)((pBuffer[0] << 24) | (pBuffer[1] << 16) | (pBuffer[2] << 8) | pBuffer[3]);

	if (m_DataManage.InitPersonalize() && m_DataManage.LoadJobFile())
	{
		if (m_DataManage.RequestEncryptKey(aucEncryptKey, &unEncryptKeyLen, unChipID))
		{
			if (m_pListenConnInterface->SendDataTo(aucEncryptKey, unEncryptKeyLen, unCommand))
			{
				nRet = 1;
			}
		}
	}

	if (nRet != 1)
	{
		m_unErrorCommand = (7 << 16) | E_GET_ENCRYPTKEY_FAIL;
	}

	return nRet;
}

int UpgradeSTB::SendVSCDataLen(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;

	if (pBuffer[0] == 1)
	{
		unsigned char aucStr[8] = { 0 };
		unsigned char pVscData[64 * 1024] = { 0 };
		unsigned int unVscDataLen = 0;

		if (m_DataManage.RequestVscData(pVscData, &unVscDataLen))
		{
			aucStr[0] = (unsigned char)((unVscDataLen >> 24) & 0xff);
			aucStr[1] = (unsigned char)((unVscDataLen >> 16) & 0xff);
			aucStr[2] = (unsigned char)((unVscDataLen >> 8) & 0xff);
			aucStr[3] = (unsigned char)(unVscDataLen & 0xff);

			if (m_pListenConnInterface->SendDataTo(aucStr, 4, unCommand))
			{
				nRet = 1;
			}
		}
		else
		{
			m_unErrorCommand = (7 << 16) | E_GET_VSCDATA_FAIL;
		}
	}
	else if (pBuffer[0] == 2)
	{
		m_enLedStatus.m_enStatus = E_Led_Reboot;
		m_enLedStatus.m_nCheck = 0;

		m_unErrorCommand = (7 << 16) | E_STB_REBOOT_FOR_INIT;
	}
	else
	{
		m_unErrorCommand = (7 << 16) | E_SEND_ENCRYPTKEY_FAIL;
	}

	return nRet;
}

int UpgradeSTB::SendVSCData(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;
	unsigned char aucStr[8] = { 0 };
	unsigned char pVscData[64 * 1024] = { 0 };
	unsigned int unVscDataLen = 0;

	if (m_DataManage.RequestVscData(pVscData, &unVscDataLen))
	{
		if (m_pListenConnInterface->SendDataTo(pVscData, unVscDataLen, unCommand))
		{
			nRet = 1;
		}
	}
	
	if (nRet != 1)
	{
		m_unErrorCommand = (7 << 16) | E_GET_VSCDATA_FAIL;
	}

	return nRet;
}

int UpgradeSTB::RequestPersonalizeResult(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRet = 0;
	
	if (m_pListenConnInterface->SendDataTo(unCommand))
	{
		nRet = 1;
	}

	return nRet;
}

int UpgradeSTB::RequestIFCPSize(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRe = 0;
	unsigned char aucStr[16] = { 0 };
	unsigned int unSendLen = 0;

	/*m_IFCPBuffer.m_unIFCPLen = m_DataManage.GetIFCP(pBuffer, m_IFCPBuffer.m_pIFCP, IFCP_BUFFER_SIZE);
	if (m_IFCPBuffer.m_unIFCPLen > 0)
	{
		m_IFCPBuffer.m_unChipLen = unLen;
		memcpy(m_IFCPBuffer.aucChipID, pBuffer, unLen);

		aucStr[0] = (unsigned char)((m_IFCPBuffer.m_unIFCPLen >> 24) & 0xff);
		aucStr[1] = (unsigned char)((m_IFCPBuffer.m_unIFCPLen >> 16) & 0xff);
		aucStr[2] = (unsigned char)((m_IFCPBuffer.m_unIFCPLen >> 8) & 0xff);
		aucStr[3] = (unsigned char)(m_IFCPBuffer.m_unIFCPLen & 0xff);
		unSendLen = 4;
		if (m_pListenConnInterface->SendDataTo(aucStr, unSendLen, unCommand))
		{
			nRe = 1;
		}
	}*/
	if (m_pListenConnInterface->SendDataTo( unCommand))
	{
		nRe = 1;
	}
	else
	{
		m_unErrorCommand = (1 << 16) | E_UPGRADE_GET_PEF_ERROR;
	}

	return nRe;
}

#define SEND_ONCE_SIZE (5*1024)

int UpgradeSTB::RequestIFCPData(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen)
{
	int nRe = 0;
	int unSeg = (int)((pBuffer[0] << 8) | pBuffer[1]);
	unsigned char aucStr[SEND_ONCE_SIZE + 2];

	if (unSeg * SEND_ONCE_SIZE < m_IFCPBuffer.m_unIFCPLen)
	{
		unsigned int unIndex = unSeg * SEND_ONCE_SIZE;
		unsigned int unSendLen = (m_IFCPBuffer.m_unIFCPLen > SEND_ONCE_SIZE * (unSeg + 1)) ? SEND_ONCE_SIZE : (m_IFCPBuffer.m_unIFCPLen - unIndex);
		aucStr[0] = pBuffer[0];
		aucStr[1] = pBuffer[1];
		memcpy(&aucStr[2], &m_IFCPBuffer.m_pIFCP[unIndex], unSendLen);
		if (m_pListenConnInterface->SendDataTo(aucStr, unSendLen+2, unCommand))
		{
			nRe = 1;
		}
	}

	return nRe;
}

int UpgradeSTB::GetConnectLedStatus(int & nStatus)
{
	int nRet = 0;

	m_StatusLock.WaitLock();

	if (m_enLedStatus.m_nCheck == 0)
	{
		nStatus = m_enLedStatus.m_enStatus;

		nRet = 1;
	}

	m_StatusLock.ReleaseLock();

	return nRet;
}

int UpgradeSTB::SetConnectCheckStatus(int nStatus)
{
	m_StatusLock.WaitLock();

	m_enLedStatus.m_nCheck = nStatus;

	m_StatusLock.ReleaseLock();

	return 1;
}

int UpgradeSTB::Reconnect(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (m_pListenConnInterface != NULL)
	{
		nRet = m_pListenConnInterface->Reconnect(pBuffer, nLen);

		if (nRet > 0)
		{
			m_unErrorCommand = ((6 << 16) | 4);
		}
		else
		{
			m_unErrorCommand = ((6 << 16) | 3);
		}
	}

	return nRet;
}

void UpgradeSTB::RebuiltPrintFile(void)
{
	if (m_DataManage.ConstructPrintFile())
	{
		ShellExecuteA(NULL, "open", "D:\\BarCode\\barprint.bat", NULL, NULL, SW_HIDE);
	}
	else
	{
		m_unErrorCommand = (3 << 16) | 4;
	}
}

int UpgradeSTB::SendIFCPResult(unsigned int unCommand)
{
	int nRet = 0;

	if (m_pListenConnInterface->SendDataTo(unCommand))
	{
		nRet = 1;
	}

	if (nRet != 1)
	{
		m_unErrorCommand = E_UPGRADE_IFCP_ERROR;
	}

	return nRet;
}