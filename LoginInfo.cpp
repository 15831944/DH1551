#include "LoginInfo.h"
#include <stdio.h>
#include <string.h>
#include "DataManage/ConfigData/NodeList.h"

#define LOGIN_FILE ("LoginData.txt")

LoginInfo::LoginInfo()
{
	ClearAll();
}

LoginInfo::~LoginInfo()
{
}

void LoginInfo::ClearAll(void)
{
	memset(m_acOrderID, 0, sizeof(m_acOrderID));
	memset(m_acLine, 0, sizeof(m_acLine));
	memset(m_acStep, 0, sizeof(m_acStep));
	memset(m_acPersonID, 0, sizeof(m_acPersonID));
}

unsigned int LoginInfo::ReadLoginData(char *pOrderID)
{
	unsigned int unRe = 0;

	if (pOrderID != NULL)
	{
		NodeList tempNodeList;
		XMLNode *pTempXMLNode = NULL;

		if (tempNodeList.ConstructNodeListByFile(LOGIN_FILE, strlen(LOGIN_FILE)))
		{
			pTempXMLNode = tempNodeList.GetXMLNode();
			if (pTempXMLNode != NULL)
			{
				unRe = pTempXMLNode->GetValue("ORDERID", strlen("ORDERID"), m_acOrderID, sizeof(m_acOrderID));
				unRe &= pTempXMLNode->GetValue("LINE", strlen("LINE"), m_acLine, sizeof(m_acLine));
				unRe &= pTempXMLNode->GetValue("STEP", strlen("STEP"), m_acStep, sizeof(m_acStep));
				unRe &= pTempXMLNode->GetValue("PERSONID", strlen("PERSONID"), m_acPersonID, sizeof(m_acPersonID));

				if (unRe > 0 && memcmp(m_acOrderID, pOrderID, strlen(m_acOrderID)) != 0)
				{
					unRe = 0;
				}
			}
		}
	}

	return unRe;
}

unsigned int LoginInfo::SaveLoginData(char *pOrderID, char *pLine, char *pStep, char *pPersonID)
{
	unsigned int unRe = 0;
	FILE *pFile = NULL;
	char acLoginStr[512] = { 0 };

	ClearAll();

	memcpy(m_acOrderID, pOrderID, strlen(pOrderID));
	memcpy(m_acLine, pLine, strlen(pLine));
	memcpy(m_acStep, pStep, strlen(pStep));
	memcpy(m_acPersonID, pPersonID, strlen(pPersonID));

	sprintf_s(&acLoginStr[strlen(acLoginStr)], sizeof(acLoginStr)-strlen(acLoginStr), "<ORDERID>%s</ORDERID>\r\n", pOrderID);
	sprintf_s(&acLoginStr[strlen(acLoginStr)], sizeof(acLoginStr)-strlen(acLoginStr), "<LINE>%s</LINE>\r\n", pLine);
	sprintf_s(&acLoginStr[strlen(acLoginStr)], sizeof(acLoginStr)-strlen(acLoginStr), "<STEP>%s</STEP>\r\n", pStep);
	sprintf_s(&acLoginStr[strlen(acLoginStr)], sizeof(acLoginStr)-strlen(acLoginStr), "<PERSONID>%s</PERSONID>\r\n", pPersonID);

	fopen_s(&pFile, LOGIN_FILE, "wb+");
	if (pFile != NULL)
	{
		fwrite(acLoginStr, sizeof(char), strlen(acLoginStr), pFile);
		fclose(pFile);
		unRe = 1;
	}

	return unRe;
}