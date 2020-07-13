#include "FactoryDBServerData.h"

FactoryDBServerData::FactoryDBServerData()
{
	m_unDBDataType = 0;
}

FactoryDBServerData::~FactoryDBServerData()
{
}

void FactoryDBServerData::SetBaseFiledData(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName)
{
	m_DBConn.SetBaseFiledData(OrderID, TestDevice, WorkcenterName, UserName);
}

unsigned int FactoryDBServerData::CheckOrderInfo(void)
{
	unsigned int unRet = 0;
	DBTableString *pTempDBTableString = m_DBConn.GetDBTableString();

	//检查订单信息
	if (m_DBConn.RequestFiledRules())
	{
		SetInsertTable(pTempDBTableString);

		unRet = 1;
	}
	else
	{
		m_UpgradeCommon.WriteLog("[GetFiledRule]:%s\n\r", pTempDBTableString->cstrSettingReturnMessage);
	}

	return unRet;
}

unsigned int FactoryDBServerData::CheckOrderInfo(char* OrderID, char* TestDevice, char* WorkcenterName, char* UserName)
{
	unsigned int unRet = 0;
	DBTableString *pTempDBTableString = m_DBConn.GetDBTableString();

	//检查订单信息
	m_DBConn.SetBaseFiledData(OrderID, TestDevice, WorkcenterName, UserName);

	if (m_DBConn.RequestFiledRules())
	{
		SetInsertTable(pTempDBTableString);

		unRet = 1;
	}
	else
	{
		m_UpgradeCommon.WriteLog("[GetFiledRule]:%s\n\r", pTempDBTableString->cstrSettingReturnMessage);
	}

	return unRet;
}

unsigned int FactoryDBServerData::ParseReturnData(unsigned char *pBuffer, unsigned int unLen)
{
	return m_ReturnFile.ParseReturnData(pBuffer, unLen);
}

unsigned int FactoryDBServerData::ConstructPrintFile(void)
{
	return m_ReturnFile.ConstructPrintData();
}

void FactoryDBServerData::SetReturnFileType(unsigned int unReturnFileType, char *pSTBType)
{
	//m_unDBDataType = unReturnFileType;

	//m_InsertTable.SetDBSTBType(pSTBType, strlen(pSTBType));

	//SetInsertTable();

	m_ReturnFile.SetReturnFileType(unReturnFileType);
}

void FactoryDBServerData::SetPlatForm(char *pPlatForm, int nLen)
{
	m_ReturnFile.SetPlatformID(pPlatForm, nLen);
}

unsigned int FactoryDBServerData::VerifyBarCode(unsigned char *pBuffer, int nLen, char *pBarCode, int nBarLen)
{
	return m_ReturnFile.VerifyBarCode(pBuffer, nLen, pBarCode, nBarLen);
}

unsigned int FactoryDBServerData::CheckWorkPoint(char *pLotSN, int nLotSNLen)
{
	unsigned int unRe = 0;

	if (pLotSN != NULL && nLotSNLen > 0)
	{
		//1、检查节点
		DBTableString *pTempDBTableString = m_DBConn.GetDBTableString();
		if (m_DBConn.CheckLotSNStatus(pLotSN, pTempDBTableString->cstrMesSnSTB.GetBuffer(0)))
		{
			unRe = 1;
		}
		else
		{
			m_UpgradeCommon.WriteLog("[CheckLotSN]:%s\n\r", pTempDBTableString->cstrCheckReturnMessage.GetBuffer(0));
			AfxMessageBox(pTempDBTableString->cstrCheckReturnMessage.GetBuffer(0));
		}
	}

	return unRe;
}

unsigned int FactoryDBServerData::InsertDataIntoDB(void)
{
	unsigned int unRet = 0;
	char acLotSN[64] = { 0 };

	if (m_ReturnFile.GetSNData(acLotSN, sizeof(acLotSN)) > 0)
	{
		//1、检查节点
		DBTableString *pTempDBTableString = m_DBConn.GetDBTableString();
		if (m_DBConn.CheckLotSNStatus(acLotSN, pTempDBTableString->cstrMesSnSTB.GetBuffer(0)))
		{
			//1.2 节点正确，则上传数据
			char acTestData[2 * 1024] = { 0 };

			memset(acTestData, 0, sizeof(acTestData));

			if (ConstructInsertString(acTestData, sizeof(acTestData)) > 0)
			{
				if (m_DBConn.SubmitTestDataToDB(acLotSN, pTempDBTableString->cstrMesSnSTB.GetBuffer(0), acTestData))
				{
					unRet = 1;
				}
				else
				{
					m_UpgradeCommon.WriteLog("[SubmitTestData]:%s\n\r", pTempDBTableString->cstrSubmitReturnMessage.GetBuffer(0));
				}
			}
			else
			{
				m_UpgradeCommon.WriteLog("ConstructInsertString Fail, acTestData:%s, len:%d\n\r", acTestData, strlen(acTestData));
			}
		}
		else
		{
			/*AfxMessageBox(pTempDBTableString->cstrCheckReturnMessage.GetBuffer(0));
			return 0;
			m_UpgradeCommon.WriteLog("[CheckLotSN]:%s\n\r", pTempDBTableString->cstrCheckReturnMessage.GetBuffer(0));
			//1.3 节点不对，比较数据是否相同
			if (m_DBConn.RequestFieldData(acLotSN, pTempDBTableString->cstrMesSnSTB.GetBuffer(0)))
			{
				if (CompareFiledData() == 1)//相同
				{
					unRet = 1;
				}
			}
			else*/
			{
				m_UpgradeCommon.WriteLog("[CheckLotSN]:%s\n\r", pTempDBTableString->cstrCheckReturnMessage.GetBuffer(0));
				AfxMessageBox(pTempDBTableString->cstrCheckReturnMessage.GetBuffer(0));
			}
		}
	}

	return unRet;
}

int FactoryDBServerData::CompareFiledData(void)
{
	int nRet = 0;
	DBTableString *pTempDBTableString = m_DBConn.GetDBTableString();
	unsigned int unReturnType = m_ReturnFile.GetReturnFileType();

	if ( unReturnType & E_CHIP_RT || unReturnType & E_MAC_RT )
	{
		nRet = 1;

		if ( unReturnType & E_CHIP_RT )
		{
			pTempDBTableString->cstrSqlChipID = pTempDBTableString->mesDataMap[pTempDBTableString->cstrMesChipId];
			nRet &= m_ReturnFile.CompareReturnValue(E_CHIP_RT, pTempDBTableString->cstrSqlChipID.GetBuffer(0), pTempDBTableString->cstrSqlChipID.GetLength());
		}

		if ( unReturnType & E_MAC_RT )
		{
			pTempDBTableString->cstrSqlMacEth = pTempDBTableString->mesDataMap[pTempDBTableString->cstrMesMacEth];
			nRet &= m_ReturnFile.CompareReturnValue(E_MAC_RT, pTempDBTableString->cstrSqlMacEth.GetBuffer(0), pTempDBTableString->cstrSqlMacEth.GetLength());
		}
	}

	return nRet;
}

unsigned int FactoryDBServerData::ConstructInsertString(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (m_InsertTable.SetReturnValue(&m_ReturnFile))
	{		
		unRet = m_InsertTable.GetInsertString(pBuffer, nLen);
	}

	return unRet;
}

void FactoryDBServerData::SetInsertTable(DBTableString *pTempDBTableString)
{
	std::map<CString, vector<CString> >::iterator multitr;

	for (multitr = pTempDBTableString->mesSettingMap.begin(); multitr != pTempDBTableString->mesSettingMap.end(); multitr++)
	{
		if (multitr->first == pTempDBTableString->cstrMesDBModel)//数据库机型
		{
			m_InsertTable.SetDBSTBType(pTempDBTableString->cstrSqlDBModel.GetBuffer(), pTempDBTableString->cstrSqlDBModel.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesSnSTB)//SN号（主批号，预置）
		{
			m_unDBDataType |= E_SN_RT;
			//m_InsertTable.AddOneFiled(E_SN_RT, pTempDBTableString->cstrMesSnSTB.GetBuffer(0), pTempDBTableString->cstrMesSnSTB.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesChipId)//CHIPID号
		{
			m_unDBDataType |= E_CHIP_RT;
			m_InsertTable.AddOneFiled(E_CHIP_RT, pTempDBTableString->cstrMesChipId.GetBuffer(0), pTempDBTableString->cstrMesChipId.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesMacEth)//MAC号
		{
			m_unDBDataType |= E_MAC_RT;
			m_InsertTable.AddOneFiled(E_MAC_RT, pTempDBTableString->cstrMesMacEth.GetBuffer(0), pTempDBTableString->cstrMesMacEth.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesSnGD)//CDSN
		{
			m_unDBDataType |= E_CDSN_RT;
			m_InsertTable.AddOneFiled(E_CDSN_RT, pTempDBTableString->cstrMesSnGD.GetBuffer(0), pTempDBTableString->cstrMesSnGD.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesOption1)//私有数据
		{
			m_unDBDataType |= E_PRIVATE_RT;
			m_InsertTable.AddOneFiled(E_PRIVATE_RT, pTempDBTableString->cstrMesOption1.GetBuffer(0), pTempDBTableString->cstrMesOption1.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesOption2)//PIN
		{
			m_unDBDataType |= E_PIN_RT;
			m_InsertTable.AddOneFiled(E_PIN_RT, pTempDBTableString->cstrMesOption2.GetBuffer(0), pTempDBTableString->cstrMesOption2.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesOption3)//十进制CHIPID
		{
			m_unDBDataType |= E_DIGCHIP_RT;
			m_InsertTable.AddOneFiled(E_DIGCHIP_RT, pTempDBTableString->cstrMesOption3.GetBuffer(0), pTempDBTableString->cstrMesOption3.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesOption7)//JOB ID
		{
			m_unDBDataType |= E_JOBID_RT;
			m_InsertTable.AddOneFiled(E_JOBID_RT, pTempDBTableString->cstrMesOption7.GetBuffer(0), pTempDBTableString->cstrMesOption7.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesOption8)//VSC ID
		{
			m_unDBDataType |= E_VSCID_RT;
			m_InsertTable.AddOneFiled(E_VSCID_RT, pTempDBTableString->cstrMesOption8.GetBuffer(0), pTempDBTableString->cstrMesOption8.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesCodeContent)//VALID NUM
		{
			m_unDBDataType |= E_VALID_RT;
			m_InsertTable.AddOneFiled(E_VALID_RT, pTempDBTableString->cstrMesCodeContent.GetBuffer(0), pTempDBTableString->cstrMesCodeContent.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesSnSTBID)//平台号+chipid
		{
			m_unDBDataType |= E_STBID_RT;
			m_InsertTable.AddOneFiled(E_STBID_RT, pTempDBTableString->cstrMesSnSTBID.GetBuffer(0), pTempDBTableString->cstrMesSnSTBID.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesOption9)//E_CA_SERIAL_NUM
		{
			m_unDBDataType |= E_CA_SERIAL_NUM;
			m_InsertTable.AddOneFiled(E_CA_SERIAL_NUM, pTempDBTableString->cstrMesOption9.GetBuffer(0), pTempDBTableString->cstrMesOption9.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesOption10)
		{
			m_unDBDataType |= E_CARD_ID;
			m_InsertTable.AddOneFiled(E_CARD_ID, pTempDBTableString->cstrMesOption10.GetBuffer(0), pTempDBTableString->cstrMesOption10.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesSnCard)
		{
			m_unDBDataType |= E_CARD_SN;
			m_InsertTable.AddOneFiled(E_CARD_SN, pTempDBTableString->cstrMesSnCard.GetBuffer(0), pTempDBTableString->cstrMesSnCard.GetLength());
		}
		else if (multitr->first == pTempDBTableString->cstrMesSnTemp)//临时批号
		{
			m_unDBDataType |= E_TEMPSN_RT;
		}
		else if (multitr->first == pTempDBTableString->cstrMesRandNo)//tsc
		{
			m_unDBDataType |= E_TSC_DATA;
			m_InsertTable.AddOneFiled(E_TSC_DATA, pTempDBTableString->cstrMesRandNo.GetBuffer(0), pTempDBTableString->cstrMesRandNo.GetLength());
		}
	}

	m_ReturnFile.SetReturnFileType(m_unDBDataType);
}


void FactoryDBServerData::SetInsertTable(void)
{
	DBTableString *pTempDBTableString = m_DBConn.GetDBTableString();

	if (m_unDBDataType & E_SN_RT)//SN号（主批号，预置）
	{			
		//m_InsertTable.AddOneFiled(E_SN_RT, pTempDBTableString->cstrMesSnSTB.GetBuffer(0), pTempDBTableString->cstrMesSnSTB.GetLength());
	}
	if (m_unDBDataType & E_CHIP_RT)//CHIPID号
	{			
		m_InsertTable.AddOneFiled(E_CHIP_RT, pTempDBTableString->cstrMesChipId.GetBuffer(0), pTempDBTableString->cstrMesChipId.GetLength());
	}
	if (m_unDBDataType & E_MAC_RT)//MAC号
	{		
		m_InsertTable.AddOneFiled(E_MAC_RT, pTempDBTableString->cstrMesMacEth.GetBuffer(0), pTempDBTableString->cstrMesMacEth.GetLength());
	}
	if (m_unDBDataType & E_CDSN_RT)//CDSN
	{		
		m_InsertTable.AddOneFiled(E_CDSN_RT, pTempDBTableString->cstrMesSnGD.GetBuffer(0), pTempDBTableString->cstrMesSnGD.GetLength());
	}
	if (m_unDBDataType & E_PRIVATE_RT)//私有数据
	{		
		m_InsertTable.AddOneFiled(E_PRIVATE_RT, pTempDBTableString->cstrMesOption1.GetBuffer(0), pTempDBTableString->cstrMesOption1.GetLength());
	}
	if (m_unDBDataType & E_PIN_RT)//PIN
	{		
		m_InsertTable.AddOneFiled(E_PIN_RT, pTempDBTableString->cstrMesOption2.GetBuffer(0), pTempDBTableString->cstrMesOption2.GetLength());
	}
	if (m_unDBDataType & E_DIGCHIP_RT)//十进制CHIPID
	{		
		m_InsertTable.AddOneFiled(E_DIGCHIP_RT, pTempDBTableString->cstrMesOption3.GetBuffer(0), pTempDBTableString->cstrMesOption3.GetLength());
	}
	if (m_unDBDataType & E_JOBID_RT)//JOB ID
	{		
		m_InsertTable.AddOneFiled(E_JOBID_RT, pTempDBTableString->cstrMesOption7.GetBuffer(0), pTempDBTableString->cstrMesOption7.GetLength());
	}
	if (m_unDBDataType & E_VSCID_RT)//VSC ID
	{		
		m_InsertTable.AddOneFiled(E_VSCID_RT, pTempDBTableString->cstrMesOption8.GetBuffer(0), pTempDBTableString->cstrMesOption8.GetLength());
	}
	if (m_unDBDataType & E_VALID_RT)//VALID NUM
	{		
		m_InsertTable.AddOneFiled(E_VALID_RT, pTempDBTableString->cstrMesCodeContent.GetBuffer(0), pTempDBTableString->cstrMesCodeContent.GetLength());
	}
	if (m_unDBDataType & E_STBID_RT)//平台号+chipid
	{
		m_InsertTable.AddOneFiled(E_STBID_RT, pTempDBTableString->cstrMesSnSTBID.GetBuffer(0), pTempDBTableString->cstrMesSnSTBID.GetLength());
	}

	m_ReturnFile.SetReturnFileType(m_unDBDataType);
}

unsigned int FactoryDBServerData::GetDBSTBType(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (m_InsertTable.GetDBSTBType(pBuffer, nLen))
	{
		unRet = 1;
	}

	return unRet;
}

unsigned int FactoryDBServerData::GetDBDataType(void)
{
	return m_unDBDataType;
}

unsigned int FactoryDBServerData::RequestOneRecord(char *pSN, int nLen, char *pPrintData, int nPrintLen)
{
	unsigned int unRet = 0;

	DBTableString *pTempDBTableString = m_DBConn.GetDBTableString();

	if (m_DBConn.RequestFieldData(pSN, pTempDBTableString->cstrMesSnSTB.GetBuffer(0)))
	{
		unRet = 1;
	}

	return unRet;
}