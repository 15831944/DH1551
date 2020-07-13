#ifndef XML_NODE_H_
#define XML_NODE_H_

#include <list>

using namespace std;

class XMLNode
{
public:
	XMLNode(char *pTag, int nTagLen, char *pValue, int nValLen);
	~XMLNode();

public:
	int IsSameTag(char *pTag, int nTagLen);
	int GetValue(char *pValue, int nValLen);

	XMLNode *GetXMLNode(char *pTag, int nTagLen);
	int GetValue(char *pTag, int nTagLen, char *pValue, int nValLen);
	int GetValue(int *pValue, char *pTag, int nTagLen);
	int GetValue(char *pTag, int nTagLen, int *pValue);

	int SetTag(char *pTag, int nTagLen);
	int SetValue(char *pValue, int nValLen);
	int AddNode(char *pTag, int nTagLen, char *pValue, int nValLen);
	int AddNode(XMLNode *pXMLNode);
	int WriteFiledDataToFile(char *pFileName, int nLen, char *pRecordFile, int nRecordLen);

	void PrintAll(int nDeep);

private:
	int DelAllNode(void);
	unsigned int StringToNum(char *pBuffer, int nLen, int & nTemp);
	unsigned int CharToNum(char cTemp, int nType);
	int SaveFileData(char *pBuffer, int nBufLen);
	
private:
	char m_acTag[32];
	char m_acValue[32*3];
	
	list<XMLNode*> m_pNodelist;
};

#endif/*XML_NODE_H_*/