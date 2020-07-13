#ifndef AP_LOCK_H_
#define AP_LOCK_H_

#include "afxwin.h"

class APLock
{
public:
	APLock();
	~APLock();

public:
	int WaitLock(unsigned int unMilliSecond);
	int WaitLock(void);
	int ReleaseLock(void);

private:
	HANDLE m_hMutex;
};

#endif/*AP_LOCK_H_*/