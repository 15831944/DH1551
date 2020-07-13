#ifndef PARSE_CONFIG_H_
#define PARSE_CONFIG_H_

#include "ConfigData.h"
#include "NodeList.h"

class ParseConfig
{
public:
	ParseConfig();
	~ParseConfig();

public:
	unsigned int ReadConfigFile(char *pFileName, int nLen);
	unsigned int WriteConfigFile(char *pFileName, int nLen, ConfigData csConfigData);
	int SetConfigData(XMLNode *pTempXMLNode);
	unsigned int GetConfigData(ConfigData & csConfigData);
	unsigned int SaveComChange(char * pComName, char *pFileName, int nLen);

private:
	ConfigData m_ConfigData;
};

#endif/*PARSE_CONFIG_H_*/
