#include "DBConn.h"
#include "../DBDataParse/Markup.h"

DBConn::DBConn()
{
	m_csDBTableString.SetDefaultString();

	ClearAll();
}

DBConn::DBConn(const DBConn & csDBConn)
{
	memcpy(&m_csDBTableString, &csDBConn.m_csDBTableString, sizeof(csDBConn.m_csDBTableString));
}

DBConn::~DBConn()
{
}

DBTableString *DBConn::GetDBTableString(void)
{
	return (&m_csDBTableString);
}

void DBConn::SetBaseFiledData(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName)
{
	ClearAll();

	memcpy(m_acOrderID, OrderID, strlen(OrderID));
	memcpy(m_acTestDevice, TestDevice, strlen(TestDevice));
	memcpy(m_acWorkcenterName, WorkcenterName, strlen(WorkcenterName));
	memcpy(m_acUserID, UserName, strlen(UserName));

	GetResourceName();
}

int DBConn::RequestFiledRules(void)
{
	int nRet = 0;
	char *pReturnData = NULL;

	pReturnData = m_csWebService.ADCGetInterfaceParameter("FieldRule", "", "", m_acOrderID, m_acTestDevice, m_acWorkcenterName, m_acResourceName, m_acUserID);
	if (pReturnData != NULL)
	{
		if (ParseFiledXML(pReturnData, "FieldRule"))
		{
			vector<CString> vectemp;
			vectemp = m_csDBTableString.mesSettingMap[m_csDBTableString.cstrMesReturnResult];
			if (vectemp.size() > 0)
			{
				m_csDBTableString.cstrSettingReturnResult = vectemp[0];
			}
			vectemp = m_csDBTableString.mesSettingMap[m_csDBTableString.cstrMesReturnMessage];
			if (vectemp.size() > 0)
			{
				m_csDBTableString.cstrSettingReturnMessage = vectemp[0];
			}
			vectemp = m_csDBTableString.mesSettingMap[m_csDBTableString.cstrMesDBModel];
			if (vectemp.size() > 0)
			{
				m_csDBTableString.cstrSqlDBModel = vectemp[0];
			}
			if (atoi(m_csDBTableString.cstrSettingReturnResult) == 1)//如果返回值ReturnResult不为1(成功)，则弹出错误信息
			{
				nRet = 1;
			}
			else
			{
				m_csDBTableString.cstrMesError.Format("[MES服务器]%s", m_csDBTableString.cstrSettingReturnMessage);
				AfxMessageBox(m_csDBTableString.cstrMesError);
			}
		}
	}

	return nRet;
}

int DBConn::RequestFieldData(char *pSNStr, char *pMesSNSTB)
{
	int nRet = 0;
	char *pReturnData = NULL;

	pReturnData = m_csWebService.ADCGetInterfaceParameter("FieldData", pSNStr, pMesSNSTB, m_acOrderID, m_acTestDevice, m_acWorkcenterName, m_acResourceName, m_acUserID);
	if (pReturnData != NULL)
	{
		if (ParseFiledXML(pReturnData, "FieldData"))
		{
			m_csDBTableString.cstrSqlReturnResult = m_csDBTableString.mesDataMap[m_csDBTableString.cstrMesReturnResult];
			m_csDBTableString.cstrSqlReturnMessage = m_csDBTableString.mesDataMap[m_csDBTableString.cstrMesReturnMessage];
			
			if (atoi(m_csDBTableString.cstrSqlReturnResult) == 1)//如果返回值ReturnResult不为1(成功)，则提示错误信息
			{
				nRet = 1;
			}
		}
	}

	return nRet;
}

int DBConn::RequestFieldWord(char *pSNStr, char *pMesSNSTB)
{
	int nRet = 0;
	char *pReturnData = NULL;

	pReturnData = m_csWebService.ADCGetInterfaceParameter("FieldWord", "", pMesSNSTB, m_acOrderID, m_acTestDevice, m_acWorkcenterName, m_acResourceName, m_acUserID);
	if (pReturnData != NULL)
	{
		if (ParseFiledXML(pReturnData, "FieldWord"))
		{
			m_csDBTableString.cstrSqlReturnResult = m_csDBTableString.mesGetWordMap[m_csDBTableString.cstrMesReturnResult];
			m_csDBTableString.cstrSqlReturnMessage = m_csDBTableString.mesGetWordMap[m_csDBTableString.cstrMesReturnMessage];

			if (atoi(m_csDBTableString.cstrSqlReturnResult) == 1)//如果返回值ReturnResult不为1(成功)，则提示错误信息
			{
				memcpy(pSNStr, m_csDBTableString.mesGetWordMap[pMesSNSTB], m_csDBTableString.mesGetWordMap[pMesSNSTB].GetLength());

				nRet = 1;
			}
		}
	}

	return nRet;
}

int DBConn::CheckLotSNStatus(char *pSNStr, char *pMesSNSTB)
{
	int nRet = 0;
	char *pReturnData = NULL;

	pReturnData = m_csWebService.ADCCheckLotSN("TEST", pSNStr, pMesSNSTB, m_acOrderID, m_acTestDevice, m_acWorkcenterName, m_acResourceName, m_acUserID);
	if (pReturnData != NULL)
	{
		if (ParseCheckOrSubmitXML(pReturnData))
		{
			m_csDBTableString.cstrCheckReturnResult = m_csDBTableString.mesCheckorSubmitMap[m_csDBTableString.cstrMesReturnResult];
			m_csDBTableString.cstrCheckReturnMessage = m_csDBTableString.mesCheckorSubmitMap[m_csDBTableString.cstrMesReturnMessage];
			m_csDBTableString.cstrCheckSpecificationName = m_csDBTableString.mesCheckorSubmitMap[m_csDBTableString.cstrMesSpecificationName];
			m_csDBTableString.cstrCheckProductName = m_csDBTableString.mesCheckorSubmitMap[m_csDBTableString.cstrMesProductName];
			m_csDBTableString.cstrCheckMOName = m_csDBTableString.mesCheckorSubmitMap[m_csDBTableString.cstrMesMOName];
			if (atoi(m_csDBTableString.cstrCheckReturnResult) == 1)//如果返回值ReturnResult不为1(成功)，则提示错误信息
			{
				nRet = 1;
			}
		}
	}

	return nRet;
}

int DBConn::SubmitTestDataToDB(char *pSNStr, char *pMesSNSTB, char *pTestData)
{
	int nRet = 0;
	char *pReturnData = NULL;

	GenTestDate(pTestData);//生成需要测试数据和需要插入数据库中的数据
	CMarkup xml;
	xml.Load("./mestestdata.xml");
	MCD_STR strXML = xml.GetDoc();
	char* strTestData = new char[strlen(strXML.GetBuffer(0)) + 1];
	strcpy(strTestData, (const char*)strXML.GetBuffer(0)); //Display the text result
	pReturnData = m_csWebService.ADCSubmitTestData("TEST", pSNStr, pMesSNSTB, m_acOrderID, m_acTestDevice, m_acWorkcenterName, m_acResourceName, m_acUserID, strTestData);
	delete[] strTestData;//使用完毕后要清理内存
	if (pReturnData != NULL)
	{
		if (ParseCheckOrSubmitXML(pReturnData))
		{
			m_csDBTableString.cstrSubmitReturnResult = m_csDBTableString.mesCheckorSubmitMap[m_csDBTableString.cstrMesReturnResult];
			m_csDBTableString.cstrSubmitReturnMessage = m_csDBTableString.mesCheckorSubmitMap[m_csDBTableString.cstrMesReturnMessage];
			if (atoi(m_csDBTableString.cstrSubmitReturnResult) == 1)//如果返回值ReturnResult不为1(成功)，则提示错误信息
			{
				nRet = 1;
			}
		}
	}

	return nRet;
}

int DBConn::GetResourceName(void)
{
	int nRet = 0;
	char acBuffer[MAX_PATH] = { 0 };
	DWORD dwNameLen = MAX_PATH;

	if (GetComputerName(acBuffer, &dwNameLen))
	{
		memcpy(m_acResourceName, acBuffer, strlen(acBuffer));

		nRet = 1;
	}
	else
	{
		memcpy(m_acResourceName, "DefaultResourceName", strlen("DefaultResourceName"));
	}
	
	return nRet;
}

void DBConn::ClearAll(void)
{
	memset(m_acOrderID, 0, sizeof(m_acOrderID));
	memset(m_acTestDevice, 0, sizeof(m_acTestDevice));
	memset(m_acWorkcenterName, 0, sizeof(m_acWorkcenterName));
	memset(m_acResourceName, 0, sizeof(m_acResourceName));
	memset(m_acUserID, 0, sizeof(m_acUserID));
}

int DBConn::ParseFiledXML(char *pXML, char *PostCode)
{
	CMarkup xml;
	if (!xml.SetDoc(pXML))//setdoc返回0说明xml格式无效
	{
		m_csDBTableString.cstrMesError.Format(_T("[xml格式无效]调用parserADCGetInterfaceParameterXML时\n[MES服务器]%s"), pXML);
		AfxMessageBox(m_csDBTableString.cstrMesError);
		return 0;
	}
	xml.Save(_T("./mesADCGetInterfaceParameter.xml"));
	xml.FindElem(); // root DBSET element
	xml.IntoElem(); // inside DBSET
	while (xml.FindElem(_T("RESULT")))
	{
		xml.IntoElem();
		CString strName = _T("");
		CString strData = _T("");
		if (strcmp("FieldRule", PostCode) == 0)
		{
			m_csDBTableString.mesSettingMap.clear();//先清理再使用map，以免重复使用时上一次的数据没清掉
			std::vector<CString> resultvec;
			while (xml.FindElem())
			{
				strName = xml.GetTagName();
				strData = xml.GetData();
				resultvec.clear();
				resultvec.push_back(strData);
				m_csDBTableString.mesSettingMap.insert(STRING2VECTOR::value_type(strName, resultvec));
			}
		}
		else if (strcmp("FieldData", PostCode) == 0)
		{
			m_csDBTableString.mesDataMap.clear();//先清理再使用map，以免重复使用时上一次的数据没清掉
			while (xml.FindElem())
			{
				strName = xml.GetTagName();
				strData = xml.GetData();
				m_csDBTableString.mesDataMap.insert(map<CString, CString>::value_type(strName, strData));
			}
		}
		else if (strcmp("FieldSet", PostCode) == 0)
		{
			m_csDBTableString.mesGetDataMap.clear();//先清理再使用map，以免重复使用时上一次的数据没清掉
			while (xml.FindElem())
			{
				strName = xml.GetTagName();
				strData = xml.GetData();
				m_csDBTableString.mesGetDataMap.insert(map<CString, CString>::value_type(strName, strData));
			}
		}
		else if (strcmp("FieldWord", PostCode) == 0)
		{
			m_csDBTableString.mesGetWordMap.clear();//先清理再使用map，以免重复使用时上一次的数据没清掉
			while (xml.FindElem())
			{
				strName = xml.GetTagName();
				strData = xml.GetData();
				m_csDBTableString.mesGetWordMap.insert(map<CString, CString>::value_type(strName, strData));
			}
		}

		xml.OutOfElem();
	}
	while (xml.FindElem(_T("RECORD")))
	{
		xml.IntoElem();
		CString strName = _T("");
		CString strData = _T("");

		if (strcmp("FieldRule", PostCode) == 0)
		{
			std::vector<CString> recordvec;
			while (xml.FindElem())
			{
				if (xml.GetTagName().Compare(_T("FieldName")) == 0)
				{
					strName = xml.GetData();
				}
				else
				{
					strData = xml.GetData();
					recordvec.push_back(strData);
				}
			}
			m_csDBTableString.mesSettingMap.insert(STRING2VECTOR::value_type(strName, recordvec));
			recordvec.clear();
		}
		else if (strcmp("FieldData", PostCode) == 0)
		{
			while (xml.FindElem())
			{
				strName = xml.GetTagName();
				strData = xml.GetData();
				m_csDBTableString.mesDataMap.insert(map<CString, CString>::value_type(strName, strData));
			}
		}
		else if (strcmp("FieldSet", PostCode) == 0)
		{
			while (xml.FindElem())
			{
				strName = xml.GetTagName();
				strData = xml.GetData();
				m_csDBTableString.mesGetDataMap.insert(map<CString, CString>::value_type(strName, strData));
			}
		}
		else if (strcmp("FieldWord", PostCode) == 0)
		{
			while (xml.FindElem())
			{
				strName = xml.GetTagName();
				strData = xml.GetData();
				m_csDBTableString.mesGetWordMap.insert(map<CString, CString>::value_type(strName, strData));
			}
		}

		xml.OutOfElem();
	}

	return 1;
}

int DBConn::ParseCheckOrSubmitXML(char *pXML)
{
	CMarkup xml;
	m_csDBTableString.mesCheckorSubmitMap.clear();////先清理再使用map，以免重复使用时上一次的数据没清掉
	if (!xml.SetDoc(pXML))//setdoc返回0说明xml格式无效
	{
		m_csDBTableString.cstrMesError.Format(_T("[xml格式无效]调用parserADCCheckorSubmitXML时\n[MES服务器]%s"), pXML);
		AfxMessageBox(m_csDBTableString.cstrMesError);
		return 0;
	}
	xml.Save(_T("./mesADCCheckLotSNorADCSubmitTestData.xml"));
	xml.FindElem(); // root DBSET element
	xml.IntoElem(); // inside DBSET
	while (xml.FindElem(_T("RESULT")))
	{
		xml.IntoElem();//inside RESULT
		CString strName = _T("");
		CString strData = _T("");
		while (xml.FindElem())
		{
			strName = xml.GetTagName();
			strData = xml.GetData();
			m_csDBTableString.mesCheckorSubmitMap.insert(map<CString, CString>::value_type(strName, strData));
		}
	}
	while (xml.FindElem(_T("RECORD")))
	{
		xml.IntoElem();//inside RECORD
		CString strName = _T("");
		CString strData = _T("");
		while (xml.FindElem())
		{
			strName = xml.GetTagName();
			strData = xml.GetData();
			m_csDBTableString.mesCheckorSubmitMap.insert(map<CString, CString>::value_type(strName, strData));
		}
	}
	xml.OutOfElem();

	return 1;
}

int DBConn::GenTestDate(char *pTestData)
{
	CMarkup xml;
	xml.AddElem(_T("TestData"));
	xml.IntoElem();
	m_csDBTableString.multiMapTestData.clear();//先清理再使用map，以免重复使用时上一次的数据没清掉
	CTime mtime = CTime::GetCurrentTime();
	CString tmptime = mtime.Format("%Y%m%d%X");

	if (memcmp(m_acTestDevice, "写号", strlen("写号")) == 0)//(_T("写号") == TestDevice)
	{
		m_csDBTableString.vectorTestData.clear();
		m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesTestResult, "PASS"));
		m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesTestItem, "BurnSN"));
		m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesTestTime, tmptime.GetBuffer(0)));
		m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesTestValue, "default value"));
		m_csDBTableString.multiMapTestData.insert(pair<string, vector<pair<string, string> > >(m_csDBTableString.charMesData, m_csDBTableString.vectorTestData));
	}
	else if (memcmp(m_acTestDevice, "校号", strlen("校号")) == 0)//(_T("校号") == TestDevice)
	{
		m_csDBTableString.vectorTestData.clear();
		m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesTestResult, "PASS"));
		m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesTestItem, "CheckSN"));
		m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesTestTime, tmptime.GetBuffer(0)));
		m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesTestValue, "default value"));
		m_csDBTableString.multiMapTestData.insert(pair<string, vector<pair<string, string> > >(m_csDBTableString.charMesData, m_csDBTableString.vectorTestData));
	}
	m_csDBTableString.vectorTestData.clear();
	CString test;
	test.Format(_T("%s"), pTestData);
	m_csDBTableString.vectorTestData.push_back(pair<string, string>(m_csDBTableString.charMesWriteData, test.GetBuffer(0)));
	m_csDBTableString.multiMapTestData.insert(pair<string, vector<pair<string, string> > >(m_csDBTableString.charMesWrite, m_csDBTableString.vectorTestData));

	multimap<string, vector<pair<string, string> > >::iterator multitr;
	vector<pair<string, string> >::iterator intertr;
	for (multitr = m_csDBTableString.multiMapTestData.begin(); multitr != m_csDBTableString.multiMapTestData.end(); multitr++)
	{
		xml.AddElem((multitr->first).c_str());
		xml.IntoElem();
		for (intertr = multitr->second.begin(); intertr != multitr->second.end(); intertr++)
		{
			xml.AddElem((intertr->first).c_str(), (intertr->second).c_str());
		}
		xml.OutOfElem();
	}
	xml.Save(_T("./mestestdata.xml"));

	return 1;
}