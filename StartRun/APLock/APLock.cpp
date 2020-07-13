#include "APLock.h"

APLock::APLock()
{
	char acStr[32] = { 0 };

	sprintf_s(acStr, sizeof(acStr), "0x%x", this);

	m_hMutex = CreateMutex(0, FALSE, acStr);

	ASSERT(m_hMutex != NULL);
}

APLock::~APLock()
{
	if (m_hMutex != NULL)
	{
		CloseHandle(m_hMutex);
	}
}

int APLock::WaitLock(unsigned int unMilliSecond)
{
	int nRet = 0;

	if (WaitForSingleObject(m_hMutex, unMilliSecond) == WAIT_OBJECT_0)
	{
		nRet = 1;
	}

	return nRet;
}

int APLock::WaitLock(void)
{
	return WaitLock(INFINITE);
}

int APLock::ReleaseLock(void)
{
	return ReleaseMutex(m_hMutex);
}