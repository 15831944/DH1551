#ifndef UPGRADE_STB_H_
#define UPGRADE_STB_H_

#include "UpgradeCommand.h"
#include "../DataManage/DataManage.h"
#include "ListenUart.h"
#include "ListenNetwork.h"
#include "LedStatus.h"
#include "../CommunicateMessageShow.h"
#include "IFCPBuffer.h"

class UpgradeSTB : public ThreadRun
{
public:
	UpgradeSTB();
	~UpgradeSTB();

public:
	int Init(char *pBuffer, int nLen, int nType);
	void SetConfig(ConfigData csConfigData);
	int ReadLog(char *pBuffer, int nLen);
	void Onceprocess();
	int GetConnectLedStatus(int & nStatus);
	int SetConnectCheckStatus(int nStatus);
	int Reconnect(char *pBuffer, int nLen);

	int RequestServerData(unsigned int unCommand);
	int SendServerData(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);

	int RequestReturnFile(unsigned int unCommand);
	int SendResult(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);
	unsigned int CheckWorkPoint(char *pLotSN, int nLotSNLen);
	int InsertDataIntoDB(unsigned int unCommand);

	int RequestVerifyData(unsigned int unCommand);
	int StartVerify(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);

	int StartEraseAllData(void);
	int StartRuqestReturnFile(void);
	int QuerySTBStatus(void);

	int RequestEncryptKey(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);
	int SendVSCDataLen(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);
	int SendVSCData(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);
	int RequestPersonalizeResult(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);

	int RequestIFCPSize(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);
	int RequestIFCPData(unsigned int unCommand, unsigned char *pBuffer, unsigned int unLen);

	int SetBarCode(char *pBuffer, int nLen);
	unsigned int CheckOrderInfo(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName);
	void SetConfigData(ConfigData csConfigData);
	void RebuiltPrintFile(void);
	unsigned int GetDBSTBType(char *pBuffer, int nLen);
	unsigned int GetDBDataType(void);

	int SendIFCPResult(unsigned int unCommand);

	void CloseConn(void);

private:
	int StartEraseAllData(unsigned int unCommand);
	int StartRuqestReturnFile(unsigned int unCommand);
	int QuerySTBStatus(unsigned int unCommand);
	int GetLastCommand(CommandData *pCommandData);

private:
	//ListenUart *m_pListenUart;
	ListenConnInterface *m_pListenConnInterface;

	DataManage m_DataManage;
	LedStatus m_enLedStatus;
	APLock m_StatusLock;

	unsigned int m_unLastCommand;
	unsigned int m_unCurCommand;
	unsigned int m_unErrorCommand;

	CommunicateMessageShow m_CommunicateMessageShow;
	BOOL m_bEnableVerify;

	IFCPBuffer m_IFCPBuffer;
	UpgradeCommon m_UpgradeCommon;
};

#endif/*UPGRADE_STB_H_*/