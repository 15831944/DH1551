#ifndef LOGIN_INFO_H_
#define LOGIN_INFO_H_

class LoginInfo
{
public:
	LoginInfo();
	~LoginInfo();

public:
	unsigned int ReadLoginData(char *pOrderID);
	unsigned int SaveLoginData(char *pOrderID, char *pLine, char *pStep, char *pPersonID);
	void ClearAll(void);

public:
	char m_acOrderID[32];
	char m_acLine[32];
	char m_acStep[32];
	char m_acPersonID[32];
};

#endif/*LOGIN_INFO_H_*/