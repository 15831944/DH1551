#ifndef ONE_FILED_H_
#define ONE_FILED_H_

class OneFiled
{
public:
	OneFiled(int nID, char *pTag, int nLen);
	~OneFiled();

public:
	int GetRecord(char *pBuffer, int nLen);
	int GetID(void);
	int SetValue(char *pBuffer, int nLen);

private:
	int m_nID;
	char m_acTag[64];
	char m_acValue[64*2];
};

#endif/*ONE_FILED_H_*/