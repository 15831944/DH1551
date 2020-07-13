#include "ThreadRun.h"

ThreadRun::ThreadRun()
{
	m_Exit = 1;

	m_hThread = NULL;
}

ThreadRun::~ThreadRun()
{
}

int ThreadRun::Start(void)
{
	int nRet = 0;
	DWORD dwThread;

	if (m_hThread == NULL)
	{
		m_hThread = CreateThread(0, 0, ThreadFun, this, 0, &dwThread);
		if (m_hThread != NULL)
		{
			SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);
			nRet = 1;
		}
	}

	return nRet;
}

DWORD WINAPI ThreadRun::ThreadFun(LPVOID lpThreadParameter)
{
	if (lpThreadParameter != NULL)
	{
		((ThreadRun*)lpThreadParameter)->SetExit(0);
		((ThreadRun*)lpThreadParameter)->Run();
		((ThreadRun*)lpThreadParameter)->m_APSemaphore.Release();
	}

	return 1;
}

void ThreadRun::Run(void)
{
	while ( m_Exit == 0 )
	{
		Onceprocess();
	}
}

void ThreadRun::Stop(void)
{
	if (m_Exit == 0)
	{
		m_Exit = 1;
		m_APSemaphore.Wait();
	}
}

void ThreadRun::SetExit(int nExit)
{
	m_Exit = nExit;
}