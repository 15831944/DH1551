#ifndef STB_CONN_INTERFACE_H_
#define STB_CONN_INTERFACE_H_

class STBConnInterface
{
public:
	STBConnInterface();
	virtual ~STBConnInterface();

public:
	virtual int StartConn(char *pBuffer, unsigned int unData) = 0;
	virtual int SendDataToSTB(unsigned char *pBuffer, unsigned int unLen) = 0;
	virtual int RecvDataFromSTB(unsigned char *pBuffer, unsigned int unLen) = 0;
	virtual int CloseConn(void) = 0;
	virtual int ResetConn(void);
	virtual int IsGetConn(void);
};

#endif/*STB_CONN_INTERFACE_H_*/