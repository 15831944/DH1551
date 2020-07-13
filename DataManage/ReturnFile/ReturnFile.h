#ifndef RETURN_FILE_H_
#define RETURN_FILE_H_

#define UPGRADE_FILE_NAME ("local_data.txt")
#define PRINT_FILE_NAME ("id_print.txt")
#define SN_LEN (32)
#define MAC_LEN (32)
#define CHIP_ID_LEN (32)
#define VALID_NUM_LEN (128)
#define COMMON_LENGTH (32)

typedef enum
{
	E_NONE_TYPE = 0x0,
	E_CHIP_RT = 0x1,
	E_SN_RT = 0x2,
	E_MAC_RT = 0x4,
	E_CDSN_RT = 0x8,
	E_PRIVATE_RT = 0x10,
	E_PIN_RT = 0x20,
	E_DIGCHIP_RT = 0x40,
	E_JOBID_RT = 0x80,
	E_VSCID_RT = 0x100,
	E_VALID_RT = 0x200,
	E_STBID_RT = 0x400,
	E_TEMPSN_RT = 0x800,
	E_CA_SERIAL_NUM = 0x1000,
	E_CARD_ID = 0x2000,
	E_CARD_SN = 0x4000,
	E_TSC_DATA = 0x8000,
	E_MAX_RT=0xffff0000
}ReturnFileType_S;

class ReturnFile
{
public:
	ReturnFile();
	~ReturnFile();

public:
	void SetPlatformID(char *pBuffer, int nLen);
	void SetReturnFileType(unsigned int unReturnFileType);
	unsigned int GetReturnFileType(void);
	unsigned int ParseReturnData(unsigned char *pBuffer, unsigned int unLen);
	unsigned int ConstructPrintData(void);
	unsigned int VerifyBarCode(unsigned char *pBuffer, int nLen, char *pBarCode, int nBarLen);
	unsigned int GetValuesByType(int nType, char *pBuffer, int nLen);
	unsigned int GetSNData(char *pBuffer, int nLen);
	unsigned int GetChipIDData(char *pBuffer, int nLen);
	unsigned int GetMACData(char *pBuffer, int nLen);
	unsigned int GetCDSNData(char *pBuffer, int nLen);
	unsigned int GetPrivateData(char *pBuffer, int nLen);
	unsigned int GetPINData(char *pBuffer, int nLen);
	unsigned int GetDigitChipIDData(char *pBuffer, int nLen);
	unsigned int GetJobID(char *pBuffer, int nLen);
	unsigned int GetVSCID(char *pBuffer, int nLen);
	unsigned int GetValidNum(char *pBuffer, int nLen);
	unsigned int GetSTBID(char *pBuffer, int nLen);
	unsigned int GetCASerialNum(char *pBuffer, int nLen);
	unsigned int GetCardID(char *pBuffer, int nLen);
	unsigned int GetCardSN(char *pBuffer, int nLen);
	unsigned int GetTscData(char *pBuffer, int nLen);
	unsigned int CompareReturnValue(int nType, char *pBuffer, int nLen);

private:
	unsigned int CalculatePIN(char *pSN, int unLen);
	unsigned int GetCDSN(unsigned int unChipID, char acCDSN[], unsigned int unLen);
	unsigned char CalculateCheckDigit(char * pSN, unsigned char bLength);
	void AdditionAB(char *a, char *b, char* c);
	void Clearup(void);

private:
	unsigned int m_unReturnFileType;
	char m_acSN[SN_LEN];
	char m_acMac[MAC_LEN];
	char m_acChipID[CHIP_ID_LEN];
	char m_acPlatFormID[16];
	unsigned long long m_unChipID;
	unsigned int m_unVSCID;
	unsigned int m_unJobID;
	unsigned char m_aucValidNum[VALID_NUM_LEN];
	unsigned int m_unValidLen;
	char m_acCASerialNum[COMMON_LENGTH];
	char m_acCardID[COMMON_LENGTH];
	char m_acCardSN[COMMON_LENGTH];
	char m_acTscData[COMMON_LENGTH*2];
};

#endif/*RETURN_FILE_H_*/