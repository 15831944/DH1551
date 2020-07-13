#ifndef MAC_H_
#define MAC_H_

class MAC
{
public:
	MAC();
	~MAC();

public:
	unsigned int GetMac(char *pBuffer, int nLen);
	void SetMac(char *pBuffer, int nLen);
	void Clear(void);
	int GetUsedStatus(void);
	void SetUsedStatus(int nStatus);

private:
	char m_acMac[32];
	
	int m_EnableUsed;
};

#endif/*MAC_H_*/