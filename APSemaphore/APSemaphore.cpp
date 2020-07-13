#include "APSemaphore.h"
#include <stdio.h>
#include <afxwin.h>

APSemaphore::APSemaphore(unsigned int unCount)
{
	char acStr[32];

	sprintf(acStr, "0x%x", this);

	m_hSemaphore = CreateSemaphore(0, 0, unCount, acStr);
}

APSemaphore::~APSemaphore()
{
	CloseHandle((HANDLE)m_hSemaphore);
}

unsigned int APSemaphore::Wait(unsigned int unTimeOut)
{
	unsigned int unRe = 0;

	DWORD dwRet = WaitForSingleObject((HANDLE)m_hSemaphore, unTimeOut);

	if (dwRet != WAIT_FAILED && dwRet != WAIT_TIMEOUT)
	{
		unRe = 1;
	}

	return unRe;
}

unsigned int APSemaphore::Wait()
{
	unsigned int unRe = 0;

	DWORD dwRet = WaitForSingleObject((HANDLE)m_hSemaphore, INFINITE);

	if (dwRet != WAIT_FAILED && dwRet != WAIT_TIMEOUT)
	{
		unRe = 1;
	}

	return unRe;
}

unsigned int APSemaphore::Release()
{
	unsigned int unRe = 0;

	if (ReleaseSemaphore((HANDLE)m_hSemaphore, 1, NULL))
	{
		unRe = 1;
	}

	return unRe;
}
