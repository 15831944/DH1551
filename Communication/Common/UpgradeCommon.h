#ifndef UPGRADECOMMON_H_
#define UPGRADECOMMON_H_

#include "windows.h"

#define DATA_HEAD 0xEA
#define MIN_DATA_SIZE 9

class UpgradeCommon
{
public:
	UpgradeCommon();
	virtual ~UpgradeCommon();

	DWORD CalculateCRC32(BYTE * pbData, unsigned int unLength);

	unsigned int CheckCRC32(unsigned char aucStr[], unsigned int unLength);

	unsigned int IsIPValid(const char * pacStr, unsigned int unLength);

	void WriteLog(char * format, ...);

public:

};

#endif /* UPGRADECOMMON_H_ */