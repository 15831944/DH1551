#include "InsertTable.h"

InsertTable::InsertTable()
{
}

InsertTable::~InsertTable()
{
	DeleteFiled();
}

int InsertTable::AddOneFiled(int nID, char *pTag, int nTagLen)
{
	int nRet = 0;

	if (!CheckIsSameItem(nID))
	{
		OneFiled *pOneFiled = new OneFiled(nID, pTag, nTagLen);
		if (pOneFiled != NULL)
		{
			m_vtFiled.push_back(pOneFiled);
			nRet = 1;
		}
	}

	return nRet;
}

int InsertTable::CheckIsSameItem(int nID)
{
	int nRe = 0;

	for (int ii = 0; ii < m_vtFiled.size(); ii++)
	{
		if (m_vtFiled[ii]->GetID() == nID)
		{
			nRe = 1;
			break;
		}
	}

	return nRe;
}

int InsertTable::SetDBSTBType(char *pType, int nLen)
{
	int nRet = 0;

	if (pType != NULL && nLen < sizeof(m_acDBSTBType))
	{
		memset(m_acDBSTBType, 0, sizeof(m_acDBSTBType));
		memcpy(m_acDBSTBType, pType, nLen);
		nRet = 1;
	}

	return nRet;
}

void InsertTable::DeleteFiled(void)
{
	vector<OneFiled*>::iterator cur;

	for (cur = m_vtFiled.begin(); cur != m_vtFiled.end(); cur++)
	{
		if (*(cur) != NULL)
		{
			delete *(cur);
		}
	}

	m_vtFiled.clear();
}

int InsertTable::SetReturnValue(ReturnFile *pReturnFile)
{
	int nRet = 0;

	if (pReturnFile != NULL)
	{
		char acValue[64*2] = { 0 };
		vector<OneFiled*>::iterator cur;

		for (cur = m_vtFiled.begin(); cur != m_vtFiled.end(); cur++)
		{
			memset(acValue, 0, sizeof(acValue));
			if (pReturnFile->GetValuesByType((*cur)->GetID(), acValue, sizeof(acValue)) == 0)
			{
				break;
			}
			else
			{
				(*cur)->SetValue(acValue, strlen(acValue));
			}
		}

		if (cur == m_vtFiled.end())
		{
			nRet = 1;
		}
	}

	return nRet;
}

int InsertTable::GetInsertString(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (pBuffer != 0 && nLen > 0)
	{
		int nIndex = 0;
		vector<OneFiled*>::iterator cur;

		for (cur = m_vtFiled.begin(); cur != m_vtFiled.end(); cur++)
		{
			if ((*cur)->GetRecord(&pBuffer[nIndex], nLen - nIndex) > 0)
			{
				nIndex = strlen(pBuffer);
			}
			else
			{
				break;
			}
		}

		if (cur == m_vtFiled.end())
		{
			nRet = 1;

			if (nIndex > 0)
			{
				pBuffer[nIndex - 1] = '\0';
			}
		}
	}

	return  nRet;
}

int InsertTable::GetDBSTBType(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (strlen(m_acDBSTBType) > 0 && nLen >= strlen(m_acDBSTBType))
	{
		memcpy(pBuffer, m_acDBSTBType, strlen(m_acDBSTBType));

		nRet = 1;
	}

	return nRet;
}