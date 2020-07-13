#ifndef DATA_MANAGE_H_
#define DATA_MANAGE_H_

#include "FactoryDBServerData/FactoryDBServerData.h"
#include "CastpalServerData/CastpalServerData.h"
#include "CastpalLocalData/PEF.h"
#include "ConfigData/ConfigData.h"
#include "ConaxPersonalize/ConaxPersonalize.h"

typedef enum
{
	E_NULL_REQ = 0x0,
	E_SN_REQ = 0x1,
	E_HDCP_REQ = 0x2,
	E_MAC_REQ = 0x4,
	E_PEF_REQ = 0x8,

	E_MAX_REQ = 0xffff0000
}REQ_Type;

class DataManage
{
public:
	DataManage();
	~DataManage();

public:
	unsigned int RequestDataFromCastpalServer();
	unsigned int RegisterCastpalClient();
	unsigned int CheckWorkPoint(char *pLotSN, int nLotSNLen);
	unsigned int InsertDataIntoDB(void);
	unsigned int CheckOrderInfo(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName);
	unsigned int GetSN(char *pBuffer, int nLen);
	int GetSNLength(void);
	unsigned int GetHDCP(unsigned char *pBuffer, unsigned int unLen);
	unsigned int GetMAC(char *pBuffer, int nLen);
	unsigned int GetPEF(unsigned char *pChipID, unsigned char *pBuffer, unsigned int unLen);
	unsigned int GetIFCP(unsigned char *pChipID, unsigned char *pBuffer, unsigned int unLen);
	unsigned int ParseReturnData(unsigned char *pBuffer, unsigned int unLen);
	unsigned int ConstructPrintFile(void);
	void SetBarCode(char *pBuffer, int nLen);
	unsigned int GetBarCode(char *pBuffer, int nLen);
	void ClearAll(void);
	unsigned int VerifyBarCode(unsigned char *pBuffer, int nLen);
	void SetConfigData(ConfigData csConfigData);
	unsigned int GetReqType(void);
	unsigned int InitPersonalize(void);
	unsigned int LoadJobFile(void);
	unsigned int RequestEncryptKey(unsigned char *pBuffer, unsigned int *punLen, unsigned int unChipID);
	unsigned int RequestVscData(unsigned char *pBuffer, unsigned int *punLen);
	unsigned int GetBaseRequestData(unsigned char *pBuffer, unsigned int & unLen, unsigned char *pChipid, unsigned int unChipLen);

	unsigned int GetDBSTBType(char *pBuffer, int nLen);
	unsigned int GetDBDataType(void);

private:
	PEF m_PEF;
	CastpalServerData m_CastpalServerData;
	FactoryDBServerData m_FactoryDBServerData;
	int m_nDataType;
	char m_acOrder[32];
	BOOL m_bUsedData;
	char m_acBarCode[32];
	unsigned int m_ReqType;

	ConfigData m_ConfigData;

	ConaxPersonalize m_ConaxPersonalize;
};

#endif/*DATA_MANAGE_H_*/