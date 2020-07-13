#include "HDCP.h"
#include "string.h"

HDCP::HDCP()
{
	memset(m_acHDCPHead, 0, sizeof(m_acHDCPHead));

	Clear();
}

HDCP::~HDCP()
{
}

void HDCP::SetHDCP(unsigned char *pBuffer, unsigned int unLen)
{
	if (pBuffer != NULL && unLen <= sizeof(m_aucHDCP))
	{
		memcpy(m_aucHDCP, pBuffer, unLen);

		m_EnableUsed = 1;
	}
}

unsigned int HDCP::GetHDCPKey(unsigned char *pBuffer, unsigned int unLen)
{
	unsigned int unRet = 0;

	if (pBuffer != NULL && unLen >= HDCP_LEN)
	{
		memcpy(pBuffer, m_aucHDCP, HDCP_LEN);

		unRet = HDCP_LEN;
	}

	return unRet;
}

void HDCP::SetHDCPHead(char *pBuffer, int nLen)
{
	if (pBuffer != NULL && nLen <= sizeof(m_acHDCPHead))
	{
		memcpy(m_acHDCPHead, pBuffer, nLen);
	}
}

void HDCP::Clear(void)
{
	memset(m_aucHDCP, 0, sizeof(m_aucHDCP));

	m_EnableUsed = 0;
}

int HDCP::GetUsedStatus(void)
{
	return m_EnableUsed;
}

void HDCP::SetUsedStatus(int nStatus)
{
	m_EnableUsed = nStatus;
}