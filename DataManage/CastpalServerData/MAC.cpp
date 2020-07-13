#include "MAC.h"
#include "string.h"

MAC::MAC()
{
	Clear();
}

MAC::~MAC()
{
}

unsigned int MAC::GetMac(char *pBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL && nLen > strlen(m_acMac))
	{
		memcpy(pBuffer, m_acMac, strlen(m_acMac));

		unRet = strlen(m_acMac);
	}

	return unRet;
}

void MAC::SetMac(char *pBuffer, int nLen)
{
	if (pBuffer != NULL && nLen < sizeof(m_acMac))
	{
		memcpy(m_acMac, pBuffer, nLen);

		m_EnableUsed = 1;
	}
}

void MAC::Clear(void)
{
	memset(m_acMac, 0, sizeof(m_acMac));

	m_EnableUsed = 0;
}

int MAC::GetUsedStatus(void)
{
	return m_EnableUsed;
}

void MAC::SetUsedStatus(int nStatus)
{
	m_EnableUsed = nStatus;
}