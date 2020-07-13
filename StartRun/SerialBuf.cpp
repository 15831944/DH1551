#include "SerialBuf.h"

SerialBuf::SerialBuf()
{
	ClearBuf();
}

SerialBuf::~SerialBuf()
{

}

void SerialBuf::ClearBuf()
{
	m_unDataLen = 0;

	memset(m_pBuf, 0, sizeof(m_pBuf));

	memset(m_pReadBuffer, 0, sizeof(m_pReadBuffer));

	memset(m_pWriteBuffer, 0, sizeof(m_pWriteBuffer));
}

unsigned int SerialBuf::CheckCRC32()
{
	DWORD dwCRC32 = 0;
	DWORD dwOrgCRC32 = 0;
	unsigned int unRet = 0;
	unsigned int unDataIndex = 0;
	unsigned int unDataLen = 0;

	if (m_unDataLen < MIN_DATA_LEN)
	{
		return unRet;
	}

	//HEDA_FLAG(1B) + Command(1B)
	unDataIndex += 2;

	unDataLen = (m_pBuf[unDataIndex] << 8) | m_pBuf[unDataIndex + 1];
	unDataIndex += (unDataLen + 2);

	dwCRC32 = ((DWORD)m_pBuf[unDataIndex] << 24) | ((DWORD)m_pBuf[unDataIndex + 1] << 16) | ((DWORD)m_pBuf[unDataIndex + 2] << 8) | ((DWORD)m_pBuf[unDataIndex + 3]);
	dwOrgCRC32 = m_UpgradeCommon.CalculateCRC32(m_pBuf, unDataLen + 4);
	if (dwOrgCRC32 == dwCRC32)
	{
		unRet = 1;
	}
	return unRet;
}