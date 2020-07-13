#ifndef UPGRADE_H_
#define UPGRADE_H_

#include "../DataManage/DataManage.h"
#include "UpgradeCommand.h"
#include "UpgradeMode.h"
#include "ListenUart.h"

typedef enum
{
	E_UART_CONN,
	E_NET_CONN
};

class Upgrade
{
public:
	Upgrade(HWND hWnd);
	~Upgrade();

public:
	BOOL Start(void);
	void OpenConn(char acComName[], unsigned int unLength);
	void CloseConn(void);
	void SetBarCode(char *pBuffer, int nLen);
	int GetSNLength(void);
	unsigned int RegisterClient(void); 
	void SetConfigData(ConfigData csConfigData);
	BOOL GetPrintFlag(void);
	void RebuiltPrintFile(void);
	unsigned int CreateConnInterface(int nType);
	unsigned int CheckOrderInfo(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName);
	unsigned int StartEraseFlash(void);
	unsigned int StartRequestReturnFile(void);
	void SetRepaireMode(UPGRADE_MODE nMode);

private:
	static DWORD WINAPI ThreadFun(LPVOID lpThreadParameter); 
	int Run(void);
	unsigned int RequestUpgradeData(void);
	unsigned int TrasportUpgradeData(void);
	unsigned int ConaxPersonalization(void);
	unsigned int RuqestReturnData(void);
	unsigned int InsertDataIntoDB(void);
	unsigned int StartVerifyData(void);
	unsigned int CheckAndConnect(void);
	unsigned int SendAndRecvData(unsigned char *pBuffer, unsigned int unLen);
	unsigned int SendAndReceiveUartData(UPGRADE_COMMAND enSendCOMMAND, UPGRADE_COMMAND enReceiveCOMMAND, unsigned char *pBuffer, unsigned int unLen, int unTimeOut);
	void Stop(void);

private:
	ListenUart m_ListenUart;
	BOOL m_bOpenSuccess;
	BOOL m_bRegClient;
	BOOL m_bPrintFlag;
	HWND m_hCurrentWnd;
	HANDLE m_hThread;
	DataManage m_DataManage;
	UpgradeCommon m_UpgradeCommon; 
	UPGRADE_MODE m_emMode;
	unsigned int m_unExit;
	int m_ConnType;
	CommandData m_CommanData;
};

#endif/*UPGRADE_H_*/