#ifndef NET_WORK_H_
#define NET_WORK_H_

#include <afxwin.h>

class Network
{
public:
	Network();
	virtual ~Network();

public:
	SOCKET InitSocket(void);

	int BindNetInfo(char *pIPStr, unsigned short usPort, SOCKET & stSocket);

	int ConnectNetServer(char *pIPStr, unsigned short usPort, SOCKET & stSocket);

	int IsNetworkConnect(void);

	void Disconnect(SOCKET & stSocket);
};

#endif/*NET_WORK_H_*/