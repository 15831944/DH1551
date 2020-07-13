#ifndef NODE_LIST_H_
#define NODE_LIST_H_

#include "XMLNode.h"

class NodeList
{
public:
	NodeList();
	~NodeList();

public:
	int ConstructNodeListByFile(char *pFileName, int nFileNameLen);
	int ConstructNodeListByString(char *pBuffer, int nBufLen);
	int ConstructNodeList(char *pBuffer, int nBufLen, XMLNode *pXMLNode);
	int ConstructOneNode(char *pBuffer, int nBufLen, XMLNode *pXMLNode);
	int ConstructFirstNode(char *pBuffer, int nBufLen);
	XMLNode *GetXMLNode(void);

private:
	int IsLeafNode(char *pBuffer, int nLen);
	int GetXMLFileStr(char *pFileName, int nFileNameLen, char *pBuffer, int nBufLen);
	int GetNodeName(char *pRes, int nResLen, char *pDest, int nDestLen);
	int GetNodeValue(char *pRes, int nResLen, char *pDest, int nDestLen);

private:
	XMLNode *m_pXMLNode;
	int m_ParentCount;
	int m_ChildCount;
};

#endif/*NODE_LIST_H_*/