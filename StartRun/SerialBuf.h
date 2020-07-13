#ifndef SERIALBUF_H_
#define SERIALBUF_H_

#include "../Communication/Common/UpgradeCommon.h"

#define MAX_BUF_LEN		(16*1024)
#define MIN_DATA_LEN (8)
#define HEAD_LEN (4)

/* Data Struct: HEDA_FLAG(1B) + Command(1B) + Data Length(2B) + Data + CRC32(4B)  */

class SerialBuf
{
public:
	SerialBuf();
	virtual ~SerialBuf();

public:
	void ClearBuf();
	unsigned int CheckCRC32();

public:
	unsigned int m_unDataLen;
	BYTE m_pBuf[MAX_BUF_LEN];
	BYTE m_pReadBuffer[MAX_BUF_LEN];
	BYTE m_pWriteBuffer[MAX_BUF_LEN];
	UpgradeCommon m_UpgradeCommon;
};

#endif /* SERIALBUF_H_ */