#ifndef SN_H_
#define SN_H_

#define SN_LEN (32)

class SN
{
public:
	SN();
	~SN();

public:
	void SetSNLength(int nLen);
	int GetSNLength(void);
	int SetSN(char *pSN, int nLen);
	int GetSN(char *pSN, int nLen);
	void Clear(void);
	int GetUsedStatus(void);
	void SetUsedStatus(int nStatus);

private:
	int m_SNLen;
	char m_acSN[SN_LEN];

	int m_EnableUsed;
};

#endif/*SN_H_*/