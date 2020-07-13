#include "NodeList.h"

NodeList::NodeList()
{
	m_ParentCount = 0;

	m_ChildCount = 0;

	m_pXMLNode = NULL;
}

NodeList::~NodeList()
{
	if (m_pXMLNode != NULL)
	{
		delete m_pXMLNode;

		m_pXMLNode = NULL;
	}
}

int NodeList::ConstructNodeListByString(char *pBuffer, int nBufLen)
{
	int nRet = 0;
	int nLen = 0;

	do
	{
		if (pBuffer == NULL || nBufLen == 0)
		{
			break;
		}

		nLen = ConstructFirstNode(pBuffer, nBufLen);
		if (nLen == 0)
		{
			break;
		}

		if (ConstructNodeList(&pBuffer[nRet], nBufLen - nRet, m_pXMLNode) == 0)
		{
			break;
		}

		nRet = 1;
	} while (0);
	
	return nRet;
}

int NodeList::ConstructNodeListByFile(char *pFileName, int nFileNameLen)
{
	int nRet = 0;
	char acAllData[5 * 1024] = { 0 };
	int nAllDataLen = 0;

	do
	{
		if (pFileName == NULL || nFileNameLen == 0)
		{
			break;
		}

		nAllDataLen = GetXMLFileStr(pFileName, nFileNameLen, acAllData, sizeof(acAllData));
		if (nAllDataLen == 0)
		{
			break;
		}

		m_pXMLNode = new XMLNode("root", strlen("root"), NULL, 0);
		if (m_pXMLNode == NULL)
		{
			break;
		}

		if (ConstructNodeList(acAllData, nAllDataLen, m_pXMLNode) == 0)
		{
			break;
		}

		nRet = 1;
	} while (0);
	
	return nRet;
}

int NodeList::ConstructFirstNode(char *pBuffer, int nBufLen)
{
	int nRet = 0;
	int nNodeType = 0;
	char acNodeName[32] = { 0 };
	char acValue[32] = { 0 };

	if (pBuffer != 0 && nBufLen > 0)
	{
		for (int ii = 0; ii < nBufLen - 1; ii++)
		{
			if (pBuffer[ii] == '<' && pBuffer[ii + 1] != '/')
			{
				ii++;
				nNodeType = IsLeafNode(&pBuffer[ii], nBufLen - ii); 
				if (nNodeType == 0)
				{
					break;
				}
				nRet = GetNodeName(&pBuffer[ii], nBufLen - ii, acNodeName, sizeof(acNodeName));
				if(nRet == 0)
				{
					break;
				}
				
				ii += ( nRet + 1 );
				if(nNodeType == 1)
				{
					nRet = GetNodeValue(&pBuffer[ii], nBufLen - ii, acValue, sizeof(acValue));
					ii += ( nRet + strlen(acNodeName) + 3 );
				}
				
				m_pXMLNode = new XMLNode(acNodeName, strlen(acNodeName), acValue, strlen(acValue));
				nRet = ii;
				break;
			}
		}
	}

	return nRet;
}

XMLNode *NodeList::GetXMLNode(void)
{
	return m_pXMLNode;
}

int NodeList::ConstructNodeList(char *pBuffer, int nBufLen, XMLNode *pXMLNode)
{
	int nRet = 0;
	int nNodeType = 0;
	int ii = 0;

	if (pBuffer != 0 && nBufLen > 0)
	{
		for (ii = 0; ii < nBufLen - 1; ii++)
		{
			if (pBuffer[ii] == '<' && pBuffer[ii + 1] != '/')
			{
				nNodeType = IsLeafNode(&pBuffer[ii + 1], nBufLen - ii - 1); 
				if (nNodeType == 0)
				{
					break;
				}
				else if (nNodeType == 1)
				{
					m_ChildCount++;
					nRet = ConstructOneNode(&pBuffer[ii + 1], nBufLen - ii - 1, pXMLNode);
					if (nRet == 0)
					{
						break;
					}
					ii += (nRet + 1);
				}
				else if (nNodeType == 2)
				{
					m_ParentCount++;
					char acTag[32] = { 0 };
					XMLNode *pTemp = NULL;
					nRet = GetNodeName(&pBuffer[ii + 1], nBufLen - ii - 1, acTag, sizeof(acTag));
					
					pTemp = new XMLNode(acTag, strlen(acTag), NULL, 0);
					pXMLNode->AddNode(pTemp);

					nRet = ConstructNodeList(&pBuffer[ii + 1], nBufLen - ii - 1, pTemp);
					if (nRet == 0)
					{
						break;
					}
					ii += nRet;
				}
			}
			else if (pBuffer[ii] == '<' && pBuffer[ii + 1] == '/')
			{
				char acTag[32] = { 0 };
				nRet = GetNodeName(&pBuffer[ii + 2], nBufLen - ii - 2, acTag, sizeof(acTag));

				if (nRet == 0)
				{
					break;
				}

				if (pXMLNode->IsSameTag(acTag, strlen(acTag)))
				{
					nRet = (ii + 2 + strlen(acTag) + 1);
					break;
				}
			}
		}
	}

	nRet = ii;

	return nRet;
}

int NodeList::ConstructOneNode(char *pBuffer, int nBufLen, XMLNode *pXMLNode)//leaf
{
	char acNodeName[32] = { 0 };
	char acValue[32*3] = { 0 };
	int nLen = 0;
	int nRet = 0;

	do
	{
		nRet = GetNodeName(pBuffer, nBufLen, acNodeName, sizeof(acNodeName));
		if (nRet == 0)
		{
			break;
		}
		nLen += (nRet + 1);

		nRet = GetNodeValue(&pBuffer[nLen], nBufLen - nLen, acValue, sizeof(acValue));
		if (nRet == 0)
		{
			//break;
		}
		nLen += nRet;

		pXMLNode->AddNode(acNodeName, strlen(acNodeName), acValue, strlen(acValue));

		nRet = nLen;
	} while (0);

	return nRet;
}

int NodeList::IsLeafNode(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (pBuffer != NULL && nLen > 0)
	{
		int ii = 0;
		while (ii < nLen-1)
		{
			if (pBuffer[ii] == '<')
			{
				if (pBuffer[ii + 1] == '/')
				{
					nRet = 1;
				}
				else
				{
					nRet = 2;
				}
				break;
			}
			ii++;
		}
	}

	return nRet;
}

int NodeList::GetXMLFileStr(char *pFileName, int nFileNameLen, char *pBuffer, int nBufLen)
{
	int nRet = 0;
	FILE *pFile = NULL;

	fopen_s(&pFile, pFileName, "rb");
	if (pFile != NULL)
	{
		nRet = fread_s(pBuffer, nBufLen, sizeof(char), nBufLen, pFile);

		fclose(pFile);
	}

	return nRet;
}

int NodeList::GetNodeName(char *pRes, int nResLen, char *pDest, int nDestLen)
{
	int ii = 0;

	while (pRes[ii] != '>')
	{
		if (ii > nDestLen)
		{
			ii = 0;
			break;
		}

		pDest[ii] = pRes[ii];
		ii++;
	}

	return ii;
}

int NodeList::GetNodeValue(char *pRes, int nResLen, char *pDest, int nDestLen)
{
	int ii = 0;

	while (pRes[ii] != '<')
	{
		if (ii > nDestLen)
		{
			ii = 0;
			break;
		}

		pDest[ii] = pRes[ii];
		ii++;
	}

	return ii;
}
