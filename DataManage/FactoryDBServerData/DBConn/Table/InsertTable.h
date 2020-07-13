#ifndef INSERT_TABLE_H_
#define INSERT_TABLE_H_

#include "OneFiled.h"
#include <vector>
#include "../../../ReturnFile/ReturnFile.h"

using namespace std;

class InsertTable
{
public:
	InsertTable();
	~InsertTable();

public:
	int AddOneFiled(int nID, char *pTag, int nTagLen);
	int SetDBSTBType(char *pType, int nLen);
	void DeleteFiled(void);
	int SetReturnValue(ReturnFile *pReturnFile);
	int GetInsertString(char *pBuffer, int nLen);
	int GetDBSTBType(char *pBuffer, int nLen);

protected:
	int CheckIsSameItem(int nID);

private:
	char m_acDBSTBType[64];
	vector<OneFiled *> m_vtFiled;
};

#endif/*INSERT_TABLE_H_*/