#include "SN.h"
#include "string.h"

SN::SN()
{
	m_SNLen = 0;

	Clear();
}

SN::~SN()
{
}

void SN::SetSNLength(int nLen)
{
	m_SNLen = nLen;
}

int SN::GetSNLength(void)
{
	return m_SNLen;
}

int SN::SetSN(char *pSN, int nLen)
{
	int nRet = 0;

	if (pSN != NULL && m_SNLen == nLen)
	{
		memcpy(m_acSN, pSN, nLen);

		m_EnableUsed = 1;
	}

	return nRet;
}

int SN::GetSN(char *pSN, int nLen)
{
	int nRet = 0;

	if (pSN != NULL && m_SNLen < nLen)
	{
		memcpy(pSN, m_acSN, m_SNLen);

		nRet = strlen(m_acSN);
	}

	return nRet;
}

void SN::Clear(void)
{
	memset(m_acSN, 0, sizeof(m_acSN));

	m_EnableUsed = 0;
}

int SN::GetUsedStatus(void)
{
	return m_EnableUsed;
}

void SN::SetUsedStatus(int nStatus)
{
	m_EnableUsed = nStatus;
}
