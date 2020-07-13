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
	std::map<CString, vector<CString> > mesSettingMap; //mes�������,�ֶ�����
	std::map<CString, CString> mesDataMap;//mes��������,�ֶ�����
	std::map<CString, CString> mesGetDataMap;//mes������������,����汾��Ӳ���汾�����ݿ���͵�
	std::map<CString, CString> mesGetWordMap;//mes����Ԥ�����ݣ��Զ���ȡ����
	std::map<CString, CString> mesCheckorSubmitMap;//mes�������,�ύ����
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

	//mes�i�I�������
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

	//mes ADCCheckLotSN()���i����ֵ
	CString cstrCheckReturnResult;//���ص�ֵ1�ǳɹ���0Ϊʧ��
	CString cstrCheckReturnMessage; //���ص���ʾ��Ϣ
	CString cstrCheckSpecificationName;//���صĹ������
	CString cstrCheckProductName;//���صĲ�ƷƷ��
	CString cstrCheckMOName;//���صĹ�����
	//mes ADCSubmitTestData()���i����ֵ
	CString cstrSubmitReturnResult;//���ص�ֵ1�ǳɹ���0Ϊʧ��
	CString cstrSubmitReturnMessage; //���ص���ʾ��Ϣ
	//mes �ύ�Ĳ���վ��
	CString cstrTestDevice;//����վ��   --FT1\FT2\FT3\FT4\FT5
};

#endif/*DB_TABLE_STRING_H_*/