#include "IFCPBuffer.h"
#include <string.h>

IFCPBuffer::IFCPBuffer()
{ 
	m_unChipLen = 0; 
	m_unIFCPLen = 0; 
	m_pIFCP = new unsigned char[IFCP_BUFFER_SIZE]; 
}
IFCPBuffer::~IFCPBuffer() 
{ 
	if (m_pIFCP != NULL) 
		delete m_pIFCP; 
}