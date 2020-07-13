#ifndef LISTEN_CONN_INTERFACE_H_
#define LISTEN_CONN_INTERFACE_H_

#include "CommandData.h"

class ListenConnInterface
{
public:
	ListenConnInterface();
	virtual ~ListenConnInterface();

public:
	virtual int StartRun(char *pBuffer, int nData) = 0;
	virtual int SendDataTo(unsigned char *pBuffer, unsigned int unBufLen, unsigned int unCommand) = 0;
	virtual int SendDataTo(unsigned int unCommand) = 0;
	virtual int GetLastCommand(CommandData *pCommandData) = 0;
	virtual int Reconnect(char *pBuffer, int nLen) = 0;
	virtual void CloseConn(void) = 0;
};

#endif/*LISTEN_CONN_INTERFACE_H_*/