#ifndef DATA_RULE_TABLE_H_
#define DATA_RULE_TABLE_H_

class DataRuleTable
{
public:
	char m_acFiledName[64];
	int m_nDigitSys;
	int m_nAllBits;
	int m_nPreFixBits;
	int m_nStartBits;
	int m_nEndBits;
	int m_nMinValue;
	int m_nMaxValue;
};

#endif/*DATA_RULE_TABLE_H_*/