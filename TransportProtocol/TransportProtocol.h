#ifndef TRANSPORT_PROTOCOL_H_
#define TRANSPORT_PROTOCOL_H_

#include "../../Communication/Common/UpgradeCommon.h"

#define MIN_HEAD_LEN (5)
#define MIN_FACTORY_LEN (MIN_HEAD_LEN+4)

class TransportProtocol
{
public:
	TransportProtocol();
	~TransportProtocol();

public:
	int ConstructData(unsigned char *pResData, unsigned int unResLen, unsigned int unCommand, unsigned char *pDestData, unsigned int unDestLen);
	int PraseData(unsigned char *pResData, unsigned int unResLen, unsigned int unCommand);
	int PraseData(unsigned char *pResData, unsigned int unResLen);

private:
	UpgradeCommon m_UpgradeCommon;
};

#endif/*TRANSPORT_PROTOCOL_H_*/