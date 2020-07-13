#ifndef DATACLIENT_H_
#define DATACLIENT_H_

#include "Network/NetClient.h"
#include "Common/UpgradeCommon.h"

class DataClient
{
public:
	DataClient();
	~DataClient();

public:
	int StartConnectCastpalServer(char * pIPStr, unsigned short usPort);
	int SendDataToCastpalServer(unsigned char *pBuffer, unsigned int unLen, unsigned int nDataType);
	int RecvDataFromCastpalServer(unsigned char *pBuffer, unsigned int unLen, unsigned int & nDataType);
	void CloseConn(void);

private:
	UpgradeCommon m_UpgradeCommon;

	NetClient m_NetClient;
};

#endif /* DATACLIENT_H_ */