#include "Network.h"
#include "stdio.h"
#include <Sensapi.h>

#pragma comment(lib, "Sensapi.lib")

#pragma comment(lib,"ws2_32.lib")//提供网络API的支持

Network::Network()
{
}

Network::~Network()
{
	WSACleanup();
}

void Network::Disconnect(SOCKET & stSocket)
{
	if (stSocket != INVALID_SOCKET)
	{
		closesocket(stSocket);

		stSocket = INVALID_SOCKET;
	}
}

int Network::BindNetInfo(char *pIPStr, unsigned short usPort, SOCKET & stSocket)
{
	int nRet = 0;
	SOCKADDR_IN stSOCKADDR;

	stSOCKADDR.sin_family = AF_INET;
	stSOCKADDR.sin_addr.s_addr = inet_addr(pIPStr);//INADDR_ANY;  
	stSOCKADDR.sin_port = htons(usPort);

	nRet = bind(stSocket,   // Socket   
		(LPSOCKADDR)&stSOCKADDR,  // address  
		sizeof(struct sockaddr));// Size of address structure 

	if (nRet == SOCKET_ERROR)
	{
		Disconnect(stSocket);
	}

	return nRet;
}

SOCKET Network::InitSocket(void)
{
	WSADATA wsaData;
	WORD wVersionRequested;
	SOCKET stSocket = INVALID_SOCKET;

	wVersionRequested = MAKEWORD(1, 1);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (wsaData.wVersion == wVersionRequested)
		{
			stSocket = socket(AF_INET,          // Address family  
				SOCK_STREAM,      // Socket type  
				IPPROTO_TCP);     // Protocol 
		}
	}
	else
	{
		WSACleanup();
	}

	return stSocket;
}

int Network::ConnectNetServer(char *pIPStr, unsigned short usPort, SOCKET & stSocket)
{
	int nRet = 0;
	LPHOSTENT lpHostEntry;
	SOCKADDR_IN stSOCKADDR;

	lpHostEntry = gethostbyname(pIPStr);

	if (lpHostEntry != NULL)
	{
		stSOCKADDR.sin_family = AF_INET;
		stSOCKADDR.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
		stSOCKADDR.sin_port = htons(usPort);

		nRet = connect(stSocket,   // Socket   
					(LPSOCKADDR)&stSOCKADDR,  // address  
					sizeof(struct sockaddr));// Size of address structure

		if (nRet == SOCKET_ERROR)
		{
			Disconnect(stSocket);
		}
		else
		{
			nRet = 1;
		}
	}

	return nRet;
}

int Network::IsNetworkConnect(void)
{
	int nRet = 0;
	DWORD dwType = 0;

	if (IsNetworkAlive(&dwType))
	{
		nRet = 1;
	}

	return nRet;
}
