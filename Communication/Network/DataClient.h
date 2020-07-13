#ifndef DATACLIENT_H_
#define DATACLIENT_H_

#include "Network.h"
#include "../Common/UpgradeCommon.h"

class DataClient : public Network
{
public:
	DataClient();
	~DataClient();

public:
	int StartConnectCastpalServer(char * pIPStr, unsigned short usPort);
	int SendDataToCastpalServer(unsigned char *pBuffer, unsigned int unLen, unsigned int nDataType);
	int RecvDataFromCastpalServer(unsigned char *pBuffer, unsigned int unLen, unsigned int & nDataType);

private:
	UpgradeCommon m_UpgradeCommon;
};

#endif /* DATACLIENT_H_ */