#ifndef CASTPAL_SERVER_DATA_H_
#define CASTPAL_SERVER_DATA_H_

#include "../../Communication/DataClient.h"
#include "SN.h"
#include "HDCP.h"
#include "MAC.h"

typedef enum
{
	E_DATATYPE_REG = 0x0,
	E_DATATYPE_SN = 0x0001,
	E_DATATYPE_MAC = 0x0002,
	E_DATATYPE_HDCP = 0x0004,

	E_DATATYPE_MAX = 0xFFFF
}DataType;

class CastpalServerData
{
public:
	CastpalServerData();
	~CastpalServerData();

public:
	unsigned int RequestDataFromServer(unsigned char *pBuffer, unsigned int unLen, DataType m_enDataType);
	void SetIPAndPort(char *pIP, int nLen, unsigned short usPort);
	int RecvAndParseData(DataType enSendDataType);
	unsigned int ParseServerData(unsigned char *pBuffer, unsigned int unLen);
	unsigned int IsExistLocalData(DataType enDataType);
	void SaveUpgradeData(char *pFilename, unsigned char *pucBuffer, unsigned int unLen);
	int ReadUpgradeFile(char *pFilename, unsigned char *pucBuffer, unsigned int unLen);
	void InvalidUpgradeFile(void);
	void SetDataType(DataType enDataType);
	unsigned int GetSN(char *pBuffer, int nLen);
	int GetSNLength(void);
	unsigned int GetHDCP(unsigned char *pBuffer, unsigned int unLen);
	unsigned int GetMAC(char *pBuffer, int nLen);
	void SetSNLength(int nLen);

private:
	DataClient m_DataClient;
	char m_acIP[32];
	unsigned short m_usPort;
	UpgradeCommon m_UpgradeCommon;
	DataType m_enDataType;
	SN m_SN;
	HDCP m_HDCP;
	MAC m_MAC;
};

#endif/*CASTPAL_SERVER_DATA_H_*/