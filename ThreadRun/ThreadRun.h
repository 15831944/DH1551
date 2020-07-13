#ifndef THREAD_RUN_H_
#define THREAD_RUN_H_

#include "afxwin.h"
#include "../APSemaphore/APSemaphore.h"

class ThreadRun
{
public:
	ThreadRun();
	~ThreadRun();

public:
	int Start(void);
	static DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);
	void Run(void);
	virtual void Onceprocess(void) = 0;
	void Stop(void);
	void SetExit(int nExit);

private:
	HANDLE m_hThread;
	int m_Exit;
	APSemaphore m_APSemaphore;
};

#endif/*THREAD_RUN_H_*/