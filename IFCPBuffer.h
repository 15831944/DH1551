#ifndef IFCP_BUFFER_H_
#define IFCP_BUFFER_H_

#define IFCP_BUFFER_SIZE (0x100000)

class IFCPBuffer
{
public:
	IFCPBuffer();
	~IFCPBuffer();

public:
	unsigned char aucChipID[16];
	unsigned int m_unChipLen;
	unsigned char *m_pIFCP;
	unsigned int m_unIFCPLen;
};

#endif/*IFCP_BUFFER_H_*/