#include "ToolVersion.h"
#include "string.h"

#define UPGRADE_TOOL ("V2.5.1.4")

int GetToolVersion(char *pBuffer, int nLen)
{
	int nRet = 0;

	if (pBuffer != NULL && nLen > strlen(UPGRADE_TOOL))
	{
		memcpy(pBuffer, UPGRADE_TOOL, strlen(UPGRADE_TOOL));

		nRet = 1;
	}

	return nRet;
}
