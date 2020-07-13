#ifndef STB_SERVER_H_
#define STB_SERVER_H_

#include "Network.h"
#include "../Common/UpgradeCommon.h"
#include "../STBConnInterface.h"
#include <afxwin.h>

class STBServer : public STBConnInterface
{
public:
	STBServer();
	~STBServer();

public:
	int StartConn(char *pBuffer, unsigned int unData);
	int SendDataToSTB(unsigned char *pBuffer, unsigned int unLen);
	int RecvDataFromSTB(unsigned char *pBuffer, unsigned int unLen);
	int IsGetConn(void);
	int CloseConn(void);
	int IsLocalNetConn(void);

private:
	int StartThread(void);
	static DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);
	void SetLock(void);
	void ReleaseLock(void);
	void CountTime(void);

private:
	Network m_Network;
	HANDLE m_hThread;
	int m_Lock;
	int m_nTimeOut;
};

#endif/*STB_SERVER_H_*/