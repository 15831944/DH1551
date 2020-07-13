#include "Upgrade.h"
#include "stdio.h"
#include "UpgradeMsg.h"
#include "../Communication/Serial/Serial.h"
#include "../Communication/Network/STBServer.h"

#define HEAD_FLAG (0xEF)
#define SEG_MAX (2000)

Upgrade::Upgrade(HWND hWnd):m_hCurrentWnd(hWnd)
{
	m_hThread = NULL;

	m_bOpenSuccess = FALSE;

	m_bRegClient = FALSE;

	m_bPrintFlag = FALSE;

	m_unExit = 1;

	m_emMode = E_UPGRADE_MODE_MAX;
}

Upgrade::~Upgrade()
{
}

unsigned int Upgrade::CreateConnInterface(int nType)
{
	return 1;
}

unsigned int Upgrade::CheckOrderInfo(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName)
{
	return m_DataManage.CheckOrderInfo(OrderID, TestDevice, WorkcenterName, UserName);
}

BOOL Upgrade::Start(void)
{
	DWORD dwThread;

	if (m_hThread == NULL)
	{
		m_hThread = CreateThread(NULL, 0, ThreadFun, this, 0, &dwThread);//创建新线程
	}

	SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);

	m_UpgradeCommon.WriteLog("*************Upgrade Start*************\r\n");

	return TRUE;
}

DWORD WINAPI Upgrade::ThreadFun(LPVOID lpThreadParameter)
{
	if (lpThreadParameter != NULL)
	{
		((Upgrade *)lpThreadParameter)->Run();
	}

	return 1;
}

int Upgrade::Run(void)
{
	while (1)
	{
		if (m_bOpenSuccess && m_bRegClient)
		{
			memset(&m_CommanData, 0, sizeof(CommandData));

			if (m_ListenUart.GetLastCommand(&m_CommanData))
			{
				switch (m_CommanData.m_nCommand)
				{
				case E_UPGRADE_COMMAND_STB_SEND_UPGRADE_COMMAND:
				{
					TrasportUpgradeData();
				}
					break;

				case E_UPGRADE_COMMAND_STB_SEND_PERSONALIZE_COMMAND:
				{
					ConaxPersonalization();
				}
					break;

				case E_UPGRADE_COMMAND_STB_SEND_WAIT_SEND_UPGRADE_RESULT:
				{
					RuqestReturnData();
				}
					break;

				case E_UPGRADE_COMMAND_STB_SEND_INSERT_DB:
				{
					InsertDataIntoDB();
				}
					break;

				case E_UPGRADE_COMMAND_STB_SEND_WAIT_VERIFY:
				{
					if (!m_bPrintFlag)
					{
						m_bPrintFlag = !m_bPrintFlag;
					}
					StartVerifyData();
				}
					break;

				case E_UPGRADE_COMMAND_STB_SEND_VERIFY_FINISHED:
					::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_VERIFY_MSG, 5);
					break;

				case E_UPGRADE_COMMAND_STB_SEND_CHIPID_ERROR:
					::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_UPGRADE_MSG, E_UPGRADE_CHIPID_ERROR);
					break;

				default:
					if (E_UART_CONN != m_ConnType)
					{
						m_ListenUart.SendDataTo(E_UPGRADE_COMMAND_END);
					}
					break;
				}
			}
			else if (m_unExit == 1)
			{
				::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_WAIT_MSG, 0);
				m_unExit = 0;
			}

			Sleep(200);
		}
		else
		{
			CheckAndConnect();

			Sleep(2000);
		}
	}

	return 1;
}

void Upgrade::Stop(void)
{
	m_UpgradeCommon.WriteLog("*************Stop*************\r\n");

	m_emMode = E_UPGRADE_MODE_MAX;

	m_unExit = 1;

	//m_pSTBConnInterface->ResetConn();

	::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_STOP_UPGRADE, 0);
}

unsigned int Upgrade::CheckAndConnect(void)
{
	if (!m_bRegClient)
	{
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_REG_CLIENT, 2);
		if (RegisterClient())
		{
			m_bRegClient = TRUE;
			::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_WAIT_MSG, 0);
		}
		else
		{
			::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_REG_CLIENT, 0);
		}
	}

	if (!m_bOpenSuccess)
	{
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_OPEN_COM, 0);
	}

	return 1;
}

void Upgrade::OpenConn(char acComName[], unsigned int unLength)
{
	if (m_ListenUart.StartRun(acComName, unLength))
	{
		m_bOpenSuccess = TRUE;
		
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_OPEN_COM_MSG, 1);
		
		m_UpgradeCommon.WriteLog("*************Open Uart or net %s SUCCESS!*************\r\n", acComName);
	}
	else
	{
		m_bOpenSuccess = FALSE;
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_OPEN_COM_MSG, 0);
		m_UpgradeCommon.WriteLog("*************Open Uart or Net %s Fail!*************\r\n", acComName);
	}
}

void Upgrade::CloseConn(void)
{
	m_bOpenSuccess = FALSE;
	m_UpgradeCommon.WriteLog("*************Close Uart *************\r\n");
}

unsigned int Upgrade::TrasportUpgradeData(void)
{
	unsigned int unRet = E_UPGRADE_FAIL;
	unsigned int unLen = 0;
	unsigned char acBuffer[16 * 1024] = { 0 };
	unsigned int unReqType = m_DataManage.GetReqType();

	do
	{
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_UPGRADE_MSG, E_UPGRADE_START);

		if (m_DataManage.RequestDataFromCastpalServer() == 0)
		{
			m_UpgradeCommon.WriteLog("*************ReadSTBData 错误*************\r\n");
			unRet = E_UPGRADE_REQUEST_DATA_FAIL;
			break;
		}

		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_UPGRADE_MSG, E_UPGRADE_WAIT_FOR_REQ_DATA);
		if ((SendAndReceiveUartData(E_UPGRADE_COMMAND_PC_SEND_ACK, E_UPGRADE_COMMAND_STB_REQ_SN, NULL, 0, 10 * 1000) == 0))
		{
			m_UpgradeCommon.WriteLog("*************Wait for STB_REQ_DATA TimeOut:10s!!!!*************\r\n");
			unRet = E_UPGRADE_WAIT_FOR_REQ_DATA_TIMEOUT;
			break;
		}
		
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_UPGRADE_MSG, E_UPGRADE_START_SEND_DATA);
		if (unReqType & E_SN_REQ)
		{
			char acSN[32] = { 0 };
			int nSNLen = 0;

			nSNLen = m_DataManage.GetSN(acSN, sizeof(acSN));
			if (nSNLen <= 0)
			{
				m_UpgradeCommon.WriteLog("*************E_UPGRADE_GET_SN_ERROR!!!!*************\r\n");
				unRet = E_UPGRADE_GET_SN_ERROR;
				break;
			}
			acBuffer[unLen] = (nSNLen >> 8) & 0xff;
			acBuffer[unLen + 1] = nSNLen & 0xff;
			unLen += 2;
			memcpy(&acBuffer[unLen], acSN, nSNLen);
			unLen += nSNLen;
		}

		if (unReqType & E_HDCP_REQ)
		{
			unsigned char aucHDCP[520] = { 0 };
			unsigned int unHDCPLen = 0;

			unHDCPLen = m_DataManage.GetHDCP(aucHDCP, sizeof(aucHDCP));
			if (unHDCPLen <= 0)
			{
				m_UpgradeCommon.WriteLog("*************E_UPGRADE_GET_HDCPKEY_ERROR!!!!*************\r\n");
				unRet = E_UPGRADE_GET_HDCPKEY_ERROR;
				break;
			}
			acBuffer[unLen] = (unHDCPLen >> 8) & 0xff;
			acBuffer[unLen + 1] = unHDCPLen & 0xff;
			unLen += 2;
			memcpy(&acBuffer[unLen], aucHDCP, unHDCPLen);
			unLen += unHDCPLen;
		}

		if (unReqType & E_MAC_REQ)
		{
			char acMac[32] = { 0 };
			int nMacLen = 0;

			nMacLen = m_DataManage.GetMAC(acMac, sizeof(acMac));
			if (nMacLen <= 0)
			{
				m_UpgradeCommon.WriteLog("*************E_UPGRADE_GET_MAC_ERROR!!!!*************\r\n");
				unRet = E_UPGRADE_GET_MAC_ERROR;
				break;
			}
			acBuffer[unLen] = (nMacLen >> 8) & 0xff;
			acBuffer[unLen + 1] = nMacLen & 0xff;
			unLen += 2;
			memcpy(&acBuffer[unLen], acMac, nMacLen);
			unLen += nMacLen;
		}

		if (unReqType & E_PEF_REQ)
		{
			unsigned char acChipID[32] = { 0 };
			unsigned char aucPEFData[16 * 1024] = { 0 };
			unsigned int unPEFLen = 0;

			memcpy(acChipID, &m_CommanData.m_acData[4], 4);
			unPEFLen = m_DataManage.GetPEF(acChipID, aucPEFData, sizeof(aucPEFData));
			if (unPEFLen <= 0)
			{
				m_UpgradeCommon.WriteLog("*************E_UPGRADE_GET_PEF_ERROR!!!!*************\r\n");
				unRet = E_UPGRADE_GET_PEF_ERROR;
				break;
			}
			acBuffer[unLen] = (unPEFLen >> 8) & 0xff;
			acBuffer[unLen + 1] = unPEFLen & 0xff;
			unLen += 2;
			memcpy(&acBuffer[unLen], aucPEFData, unPEFLen);
			unLen += unPEFLen;
		}

		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_UPGRADE_MSG, E_UPGRADE_START_SEND_DATA);
		if (m_ListenUart.SendDataTo(acBuffer, unLen, E_UPGRADE_COMMAND_PC_SEND_SN) == 0)
		{
			m_UpgradeCommon.WriteLog("*************SEND UpgradeData RESULT FAIL!!!!*************\r\n");
			break;
		}

		unRet = E_UPGRADE_SUCCESS;
	} while (0);

	Stop();

	::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_UPGRADE_MSG, unRet);

	return unRet;
}

unsigned int Upgrade::ConaxPersonalization(void)
{
	unsigned int unRet = E_PERSONALIZE_FAIL;
	unsigned char aucEncryptKey[128] = { 0 };
	unsigned int unEncryptKeyLen = 0;
	unsigned int unChipID = 0; // text--%u
	unsigned int unJobID = 0; //text--%u
	unsigned char pVscData[64*1024] = { 0 };
	unsigned int unVscDataLen = 0;
	unsigned char aucStr[8] = { 0 };
	int nCount = 0;
	char acFileName[256] = { 0 };

	do
	{
		//1、获取JOB_ID和CHIP_ID,向VSC Personalization请求EncryptKey
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_CONAX_PERSONALIZATION, E_PERSONALIZE_START);
		unChipID = (m_CommanData.m_acData[0] << 24) | (m_CommanData.m_acData[1] << 16) | (m_CommanData.m_acData[2] << 8) | m_CommanData.m_acData[3];
		//if (m_DataManage.InitJobFile(acFileName, strlen(acFileName)) == 0)
		{
			m_UpgradeCommon.WriteLog("*************Get Job ID Fail*************\r\n");
			unRet = E_GET_JOBID_FAIL;
			break;
		}
		//unJobID = m_DataManage.GetJobID();
		unRet = m_DataManage.RequestEncryptKey(aucEncryptKey, &unEncryptKeyLen, unChipID);
		if (unRet != 0)
		{
			m_UpgradeCommon.WriteLog("*************Get Encrypt Key Fail*************\r\n");
			unRet = E_GET_ENCRYPTKEY_FAIL;
			break;
		}

		//2、发送EncryptKey,等待加载EncryptKey的结果
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_CONAX_PERSONALIZATION, E_SEND_ENCRYPTKEY);
		if (SendAndReceiveUartData(E_UPGRADE_COMMAND_PC_SEND_ENCRYT_KEY, E_UPGRADE_COMMAND_STB_SEND_ENCRYT_RESULT, aucEncryptKey, unEncryptKeyLen, 10 * 1000) == 0)
		{
			m_UpgradeCommon.WriteLog("*************STB END ENCRYT RESULT TimeOut:10s*************\r\n");
			unRet = E_SEND_ENCRYPTKEY_FAIL;
			break;
		}

		//3、向VSC Personalization请求VSC Data
		if (m_CommanData.m_acData[0] == 1)
		{
			//if (m_DataManage.RequestVscData(pVscData, &unVscDataLen, unChipID) == 0)
			{
				m_UpgradeCommon.WriteLog("*************Get VSC Data Fail*************\r\n");
				unRet = E_GET_VSCDATA_FAIL;
				break;
			}
			aucStr[0] = (unsigned char)((unVscDataLen >> 24) & 0xff);
			aucStr[1] = (unsigned char)((unVscDataLen >> 16) & 0xff);
			aucStr[2] = (unsigned char)((unVscDataLen >> 8) & 0xff);
			aucStr[3] = (unsigned char)(unVscDataLen & 0xff);
		}
		else
		{
			m_UpgradeCommon.WriteLog("*************STB Load Encrypt Key Fail*************\r\n");
			break;
		}

		//4、发送VSC Data的长度，等待请求VSC
		if (SendAndReceiveUartData(E_UPGRADE_COMMAND_PC_SEND_VSCDATA_LENGTH, E_UPGRADE_COMMAND_STB_REQUEST_VSCDATA, aucStr, 4, 10 * 1000) == 0)
		{
			m_UpgradeCommon.WriteLog("*************STB REQUEST VSCDATA TimeOut:10s*************\r\n");
			unRet = E_SEND_VSCDATA_FAIL;
			break;
		}

		//5、发送VSC Data , 等待返回结果
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_CONAX_PERSONALIZATION, E_SEND_VSCDATA);
		if (SendAndReceiveUartData(E_UPGRADE_COMMAND_PC_SEND_VSCDATA, E_UPGRADE_COMMAND_STB_RECV_VSCDATA, pVscData, unVscDataLen, 15 * 1000) == 0)
		{
			m_UpgradeCommon.WriteLog("*************STB RECV VSCDATA TimeOut:15s*************\r\n");
			unRet = E_SEND_VSCDATA_FAIL;
			break;
		}

		while (nCount < 3)
		{
			memset(&m_CommanData, 0, sizeof(CommandData));
			if (m_ListenUart.GetLastCommand(&m_CommanData))
			{
				if (m_CommanData.m_nCommand == E_UPGRADE_COMMAND_STB_SEND_PERSONALIZE_RESULT)
				{
					m_UpgradeCommon.WriteLog("*************STB SEND PERSONALIZE RESULT Success*************\r\n");
					break;
				}
			}
			Sleep(500);

			nCount++;
		}

		if (nCount < 3)
		{
			unRet = E_PERSONALIZE_SUCCESS;
		}
		else
		{
			unRet = E_GET_PERSONALIZE_RESULT_FAIL;
		}
	} while (0);

	Stop();

	::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_CONAX_PERSONALIZATION, unRet);

	return unRet;
}

//CHIPID + SN + MAC
unsigned int Upgrade::RuqestReturnData(void)
{
	unsigned int unRet = 0;
	unsigned char aucStr[8] = { 0 };

	do
	{
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_RECEVIE_RETURNFILE, 2);
		if (SendAndReceiveUartData(E_UPGRADE_COMMAND_PC_SEND_REQ_UPGRADE_RESULT, E_UPGRADE_COMMAND_STB_SEND_UPGRADE_RESULT, NULL, 0, 10 * 1000) == 0)
		{
			m_UpgradeCommon.WriteLog("*************STB SEND UPGRADE RESULT FAIL!!!!*************\r\n");
			break;
		}

		if (m_CommanData.m_acData[0] == 1 && m_DataManage.ParseReturnData(&m_CommanData.m_acData[1], m_CommanData.m_nDataLen - 1) == 1)
		{
			aucStr[0] = 1;

			if ((m_ListenUart.SendDataTo(aucStr, 1, E_UPGRADE_COMMAND_PC_SEND_UPGRADE_RESULT) == 0))
			{
				m_UpgradeCommon.WriteLog("*************SEND ParseReturnFile RESULT Success!!!!*************\r\n");
				break;
			}
			m_bPrintFlag = FALSE;
		}
		else
		{
			aucStr[0] = 0;
			m_ListenUart.SendDataTo(aucStr, 1, E_UPGRADE_COMMAND_PC_SEND_UPGRADE_RESULT);
			m_UpgradeCommon.WriteLog("*************Request ReturnFile DATA FAIL!!!!*************\r\n");
			break;
		}

		unRet = 1;
	} while (0);

	Stop();

	::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_RECEVIE_RETURNFILE, unRet);

	return unRet;
}

unsigned int Upgrade::InsertDataIntoDB(void)
{
	unsigned int unRet = 0;
	unsigned char aucStr[8] = { 0 };

	::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_INSERT_DATA_INTO_DB, 2);

	if (m_DataManage.InsertDataIntoDB() == 1)
	{
		aucStr[0] = 1;

		if ((m_ListenUart.SendDataTo(aucStr, 1, E_UPGRADE_COMMAND_PC_SEND_INSERT_RESULT) != 0))
		{
			m_DataManage.ClearAll();//clear local data

			unRet = 1;
		}
		else
		{
			m_UpgradeCommon.WriteLog("*************SEND INSERT RESULT FAIL!!!!*************\r\n");
		}
	}

	if (unRet == 1 && !m_bPrintFlag)
	{
		if (m_DataManage.ConstructPrintFile() == 1)
		{
			m_bPrintFlag = TRUE;

			ShellExecuteA(NULL, "open", "D:\\BarCode\\barprint.bat", NULL, NULL, SW_HIDE);

			unRet = 1;
		}
		else
		{
			m_UpgradeCommon.WriteLog("********Fail to ConstructReturnFile*******\n");
		}
	}

	::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_INSERT_DATA_INTO_DB, unRet);

	Stop();

	Sleep(2000);

	return unRet;
}

unsigned int Upgrade::StartVerifyData(void)
{
	unsigned int unRet = 0;
	unsigned char aucStr[8] = { 0 };

	if (m_emMode == E_UPGRADE_MODE_VERIFY)
	{
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_UPGRADE_MSG, MSG_USER_MSG_START_UPGRADE);
		if (SendAndReceiveUartData(E_UPGRADE_COMMAND_PC_SEND_VERIFY_COMMNAD, E_UPGRADE_COMMAND_STB_SEND_VERIFY_DATA, NULL, 0, (10 * 1000)) > 0)
		{
			if (m_CommanData.m_acData[0] == 1 && m_DataManage.VerifyBarCode(&m_CommanData.m_acData[1], m_CommanData.m_nDataLen - 1))
			{
				unRet = 1;

				aucStr[0] = 1;
			}

			if (m_ListenUart.SendDataTo(aucStr, 1, E_UPGRADE_COMMAND_PC_SEND_VERIFY_RESULT) != 0)
			{
				::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_VERIFY_MSG, unRet);
				Sleep(3000);
			}
			else
			{
				m_UpgradeCommon.WriteLog("*************SEND Verify RESULT FAIL!!!!*************\r\n");
			}
		}
		m_bPrintFlag = FALSE;

		Stop();
	}
	else
	{
		m_ListenUart.SendDataTo(aucStr, 1, E_UPGRADE_COMMAND_PC_SEND_VERIFY_RESULT);
		::PostMessageW(m_hCurrentWnd, MSG_USER_MSG, MSG_USER_MSG_VERIFY_MSG, 3);
	}

	Sleep(1000);
	
	return unRet;
}

void Upgrade::SetBarCode(char *pBuffer, int nLen)
{
	m_emMode = E_UPGRADE_MODE_VERIFY;

	m_DataManage.SetBarCode(pBuffer, nLen);
}

int Upgrade::GetSNLength(void)
{
	return m_DataManage.GetSNLength();
}

unsigned int Upgrade::RegisterClient(void)
{
	unsigned int unRet = 0;

	unRet = m_DataManage.RegisterCastpalClient();

	if (unRet == 1)
	{
		m_bRegClient = TRUE;
	}

	return unRet;
}

void Upgrade::SetConfigData(ConfigData csConfigData)
{
	m_DataManage.SetConfigData(csConfigData);
}

BOOL Upgrade::GetPrintFlag(void)
{
	BOOL bEnable = FALSE;

	if (m_emMode == E_UPGRADE_MODE_VERIFY || m_bPrintFlag)
	{
		bEnable = TRUE;
	}

	return bEnable;
}

void Upgrade::RebuiltPrintFile(void)
{
	if (m_DataManage.ConstructPrintFile())
	{
		ShellExecuteA(NULL, "open", "D:\\BarCode\\barprint.bat", NULL, NULL, SW_HIDE);
	}
}

unsigned int Upgrade::SendAndReceiveUartData(UPGRADE_COMMAND enSendCOMMAND, UPGRADE_COMMAND enReceiveCOMMAND, unsigned char *pBuffer, unsigned int unLen, int nTimeOut)
{
	unsigned int unRet = 0;
	int nCurTime = 0;

	do
	{
		if ((nCurTime % 2000) == 0)
		{
			m_ListenUart.SendDataTo(pBuffer, unLen, enSendCOMMAND);
		}

		memset(&m_CommanData, 0, sizeof(CommandData));
		if (m_ListenUart.GetLastCommand(&m_CommanData))
		{
			if (m_CommanData.m_nCommand == enReceiveCOMMAND)
			{
				m_UpgradeCommon.WriteLog("*************UartReceiveData Success,enReceiveCOMMAND=0x%x*************\r\n", enReceiveCOMMAND);
				unRet = 1;
				break;
			}
		}


		Sleep(1000);

		nCurTime += 1000;		
	} while (nCurTime < nTimeOut);

	return unRet;
}
