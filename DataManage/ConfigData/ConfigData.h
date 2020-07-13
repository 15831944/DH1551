#ifndef CONFIG_DATA_H_
#define CONFIG_DATA_H_

class ConfigData
{
public:
	int m_Status;
	int m_BarCode;
	char m_acServerIP[16];
	int m_ServerPort;
	char m_acLocalIP[16];
	int m_LocalPort;
	char m_acPersonalIP[16];
	int m_PersonalPort;
	char m_acOrderNum[32];
	char m_acSerial[8];

	int m_nSNLen;
	int m_nDataType;

	int m_nSN;
	int m_nHDCP;
	int m_nMAC;
	int m_nPEF;
	int m_nPersonal;
	char m_acPlatID[16];

	int m_nConnType;
	char m_acProject[16];
	int m_nToolMode;

	unsigned int m_unDBDataType;
	char m_acLine[32];
	char m_acStep[32];
	char m_acPersonID[32];
	char m_acSTBType[64];
};

#endif/*CONFIG_DATA_H_*/