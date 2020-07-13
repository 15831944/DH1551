#ifndef DB_TABLE_STRING_H_
#define DB_TABLE_STRING_H_

#include <map>   
#include <vector> 
#include <string>
#include "afxwin.h"

using namespace std;

typedef std::map<CString, vector<CString> > STRING2VECTOR;


class DBTableString
{
public:
	void SetDefaultString(void);

public:
	std::map<CString, vector<CString> > mesSettingMap; //mes表格设置,字段设置
	std::map<CString, CString> mesDataMap;//mes订单数据,字段数据
	std::map<CString, CString> mesGetDataMap;//mes订单配置数据,软件版本，硬件版本，数据库机型等
	std::map<CString, CString> mesGetWordMap;//mes订单预置数据，自动获取批号
	std::map<CString, CString> mesCheckorSubmitMap;//mes检查数据,提交数据
	multimap<string, vector<pair<string, string> > >multiMapTestData;
	vector<pair<string, string> > vectorTestData;
	CString cstrMesError;
	CString cstrMesReturnResult;
	CString cstrMesReturnMessage;
	CString cstrMesDBModel;
	CString cstrMesFactoryModel;
	CString cstrMesCustomerModel;
	CString cstrMesCustomer;
	CString cstrMesHwVersion;
	CString cstrMesSwVersion;
	CString cstrMesDestination;
	CString cstrMesBatchNumber;
	CString cstrMesContractNumber;
	CString cstrMesMaterialCode;
	CString cstrMesPName;
	CString cstrMesEquipmentModel;
	CString cstrMesRecord;
	CString cstrMesSnSTBID;
	CString cstrMesSnSTB;
	CString cstrMesSnTmp;
	CString cstrMesMacEth;
	CString cstrMesCAID;
	CString cstrMesSnGD;
	CString cstrMesMacWifi2G4;
	CString cstrMesMacBluetooth;
	CString cstrMesSpecificationName;
	CString cstrMesProductName;
	CString cstrMesMOName;
	CString cstrMesOption1;
	CString cstrMesOption2;
	CString cstrMesOption3;
	CString cstrMesOption4;
	CString cstrMesOption5;
	CString cstrMesOption6;
	CString cstrMesOption7;
	CString cstrMesOption8;
	CString cstrMesOption9;
	CString cstrMesOption10;
	CString cstrMesSnTemp;
	CString cstrMesChipId;
	CString cstrMesNuId;
	CString cstrMesSPSN;
	CString cstrMesSNMO;
	CString cstrMesRand;
	CString cstrMesCodeContent;
	CString cstrMesRandNo;
	CString cstrMesSnPSU;
	CString cstrMesSnRemote;
	CString cstrMesBatch;
	CString cstrMesSnPcba;
	CString cstrMesSnCard;

	char* charMesTestResult;
	char* charMesTestItem;
	char* charMesTestTime;
	char* charMesTestValue;
	char* charMesData;
	char* charMesWriteData;
	char* charMesWrite;

	CString TestDevice;
	CString cstrLine;
	CString DlgCustomerId;
	CString cstrJobNumber;
	CString cstrorderid;

	//mes蔵綢库表设置
	CString cstrSN;
	CString cstrCA;
	CString cstrSettingReturnResult;
	CString cstrSettingReturnMessage;
	CString cstrSettingDBModel;
	CString cstrSettingFactoryModel;
	///CString cstrSettingCustomerModel;
	CString cstrSettingSnSTBIDFewHex;
	CString cstrSettingSnSTBIDAllBits;
	CString cstrSettingSnSTBIDPrefixBits;
	CString cstrSettingSnSTBIDStartBits;
	CString cstrSettingSnSTBIDEndBits;
	CString cstrSettingSnSTBIDMinValue;
	CString cstrSettingSnSTBIDMaxValue;
	CString cstrSettingMacEthFewHex;
	CString cstrSettingMacEthAllBits;
	CString cstrSettingMacEthPrefixBits;
	CString cstrSettingMacEthStartBits;
	CString cstrSettingMacEthEndBits;
	CString cstrSettingMacEthMinValue;
	CString cstrSettingMacEthMaxValue;

	CString cstrSqlReturnResult;
	CString cstrSqlReturnMessage;
	CString cstrSqlDBModel;
	CString cstrSqlSnSTBID;
	CString cstrSqlCAID;
	CString cstrSqlChipID;
	CString cstrSqlMacEth;
	CString cstrSqlSnGD;
	CString cstrSqlOption1;
	CString cstrSqlMacWifi2G4;

	//mes ADCCheckLotSN()函蔵返回值
	CString cstrCheckReturnResult;//返回的值1是成功，0为失败
	CString cstrCheckReturnMessage; //返回的提示消息
	CString cstrCheckSpecificationName;//返回的规程名称
	CString cstrCheckProductName;//返回的产品品号
	CString cstrCheckMOName;//返回的工单号
	//mes ADCSubmitTestData()函蔵返回值
	CString cstrSubmitReturnResult;//返回的值1是成功，0为失败
	CString cstrSubmitReturnMessage; //返回的提示消息
	//mes 提交的测试站点
	CString cstrTestDevice;//测试站点   --FT1\FT2\FT3\FT4\FT5
};

#endif/*DB_TABLE_STRING_H_*/