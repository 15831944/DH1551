#ifndef FACTORY_DB_SERVER_DATA_H_
#define FACTORY_DB_SERVER_DATA_H_

#include "DBConn/DBConn.h"
#include "../../Communication/Common/UpgradeCommon.h"
#include "../ReturnFile/ReturnFile.h"
#include "DBConn/Table/InsertTable.h"

class FactoryDBServerData
{
public:
	FactoryDBServerData();
	~FactoryDBServerData();

public:
	void SetBaseFiledData(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName);
	unsigned int CheckOrderInfo(void);
	unsigned int CheckOrderInfo(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName);
	unsigned int ParseReturnData(unsigned char *pBuffer, unsigned int unLen);
	unsigned int ConstructPrintFile(void);
	void SetReturnFileType(unsigned int unReturnFileType, char *pSTBType);
	void SetPlatForm(char *pPlatForm, int nLen);
	unsigned int VerifyBarCode(unsigned char *pBuffer, int nLen, char *pBarCode, int nBarLen);
	unsigned int CheckWorkPoint(char *pLotSN, int nLotSNLen);
	unsigned int InsertDataIntoDB(void);
	unsigned int GetDBSTBType(char *pBuffer, int nLen);
	unsigned int GetDBDataType(void);
	unsigned int RequestOneRecord(char *pSN, int nLen, char *pPrintData, int nPrintLen);

private:
	int CompareFiledData(void);
	unsigned int ConstructInsertString(char *pBuffer, int nLen);
	void SetInsertTable(DBTableString *pTempDBTableString);
	void SetInsertTable(void);

private:
	UpgradeCommon m_UpgradeCommon;
	DBConn m_DBConn;
	ReturnFile m_ReturnFile;
	InsertTable m_InsertTable;
	unsigned int m_unDBDataType;
};

#endif/*FACTORY_DB_SERVER_DATA_H_*/