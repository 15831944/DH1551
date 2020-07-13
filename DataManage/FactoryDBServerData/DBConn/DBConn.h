#ifndef DB_CONN_H_
#define DB_CONN_H_

#include "DBTableString.h"
#include "../WebService/WebService.h"

class DBConn
{
public:
	DBConn();
	DBConn(const DBConn & csDBConn);
	~DBConn();

public:
	void SetBaseFiledData(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName);
	int RequestFiledRules(void);
	int RequestFieldData(char *pSNStr, char *pMesSNSTB);
	int RequestFieldWord(char *pSNStr, char *pMesSNSTB);
	int CheckLotSNStatus(char *pSNStr, char *pMesSNSTB);
	int SubmitTestDataToDB(char *pSNStr, char *pMesSNSTB, char *pTestData);
	DBTableString *GetDBTableString(void);

private:
	int GetResourceName(void);
	void ClearAll(void);
	int ParseFiledXML(char *pXML, char *PostCode);
	int ParseCheckOrSubmitXML(char *pXML);
	int GenTestDate(char *pTestData);

private:
	char m_acOrderID[64];
	char m_acTestDevice[64];
	char m_acWorkcenterName[64];
	char m_acResourceName[128];
	char m_acUserID[64];

	WebService m_csWebService;
	DBTableString m_csDBTableString;
};

#endif/*DB_CONN_H_*/