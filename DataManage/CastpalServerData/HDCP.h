#ifndef HDCP_H_
#define HDCP_H_

#define HDCP_LEN (512)

class HDCP
{
public:
	HDCP();
	~HDCP();

public:
	void SetHDCP(unsigned char *pBuffer, unsigned int unLen);
	unsigned int GetHDCPKey(unsigned char *pBuffer, unsigned int unLen);
	void SetHDCPHead(char *pBuffer, int nLen);
	void Clear(void);
	int GetUsedStatus(void);
	void SetUsedStatus(int nStatus);

private:
	char m_acHDCPHead[32];
	unsigned char m_aucHDCP[HDCP_LEN];
	
	int m_EnableUsed;
};

#endif/*HDCP_H_*/