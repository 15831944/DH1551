#include "OneFiled.h"
#include "string.h"
#include "stdio.h"

OneFiled::OneFiled(int nID, char *pTag, int nLen)
{
	m_nID = nID;

	memset(m_acTag, 0, sizeof(m_acTag));
	memset(m_acValue, 0, sizeof(m_acValue));

	if (pTag != NULL && nLen < sizeof(m_acTag))
	{
		memcpy(m_acTag, pTag, nLen);
	}
}

OneFiled::~OneFiled()
{
}

int OneFiled::GetID(void)
{
	return m_nID;
}

int OneFiled::GetRecord(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (pBuffer != NULL && nLen > strlen(m_acValue) + strlen(m_acTag) + 2)
	{
		sprintf_s(pBuffer, nLen, "%s=%s,", m_acTag, m_acValue);

		nRet = strlen(m_acValue) + strlen(m_acTag) + 2;
	}

	return nRet;
}

int OneFiled::SetValue(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (pBuffer != NULL && nLen < sizeof(m_acValue))
	{
		memset(m_acValue, 0, sizeof(m_acValue));

		memcpy(m_acValue, pBuffer, nLen);

		nRet = 1;
	}

	return nRet;
}