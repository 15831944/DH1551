#include "string.h"
#include <stdio.h>
#include <afxwin.h> 
#include "PEF.h"

#define IFCP_FILE "IFCP.stb"
#define PEF_FILE "PEF.pef"

#define IFCP_PS_HEADER_BINARY_LEN 16
#define IFCP_PS_IMAGE_HEADER_BINARY_LEN 32

static unsigned short g_ImageVersion = 0;
static BOOL g_IsTestImage = FALSE;

PEF::PEF()
{
	memset(m_ucIPCP_cwodd, 0, sizeof(m_ucIPCP_cwodd));
	memset(m_unIFCP_cweven, 0, sizeof(m_unIFCP_cweven));
	memset(m_ucIFCP_Image, 0, sizeof(m_ucIFCP_Image));
	memset(m_ucIFCP_k2, 0, sizeof(m_ucIFCP_k2));

	memset(m_acLastChipID, 0, sizeof(m_acLastChipID));
	ClearAllData();
}

PEF::~PEF()
{
}

unsigned int PEF::ParseAllData(unsigned char *pucChipID, unsigned char *pBuffer, unsigned int unLen)
{
	unsigned int unRet = 0;
	unsigned char aucChipID[10] = { 0 };
	unsigned int unChipID = 0;
	unsigned int unIndex = 0;
	unsigned int pos = 0;

	do
	{
		unChipID = (pucChipID[0] << 24) | (pucChipID[1] << 16) | (pucChipID[2] << 8) | pucChipID[3];
		sprintf_s((char*)aucChipID, sizeof(aucChipID), "%02x%02x%02x%02x", pucChipID[0], pucChipID[1], pucChipID[2], pucChipID[3]);
		LittleToBig(aucChipID, sizeof(aucChipID));

		if (memcmp(m_acLastChipID, aucChipID, 4) != 0)
		{
			if (PEFInit((char*)aucChipID, strlen((char*)aucChipID)) == 0)
			{
				break;
			}
		}

		if (ParsePEFData(aucChipID, unChipID) == 0)
		{
			break;
		}

		if (ParseIFCPData(aucChipID, unChipID) == 0)
		{
			break;
		}

		pos = 0;
		unIndex = GetPEFData(&pBuffer[2], unLen - 2);
		if (unIndex == 0)
		{
			break;
		}
		pBuffer[pos] = (unIndex >> 8) & 0xff;
		pBuffer[pos + 1] = unIndex & 0xff;
		pos += 2;
		pos += unIndex;

		unIndex = GetIFCPData(&pBuffer[pos + 2], unLen - pos - 2);
		if (unIndex == 0)
		{
			break;
		}
		pBuffer[pos] = (unIndex >> 8) & 0xff;
		pBuffer[pos + 1] = unIndex & 0xff;
		pos += (unIndex + 2);
		//unLen = pos;

		unRet = pos;
	} while (0);

	return unRet;
}

unsigned int PEF::ParseAllData(unsigned char *pucChipID, unsigned char *pBuffer, unsigned int unLen, DATATYPE eDataType)
{
	unsigned int unRet = 0;
	unsigned char aucChipID[10] = { 0 };
	unsigned int unChipID = 0;
	unsigned int unIndex = 0;
	unsigned int pos = 0;

	do
	{
		unChipID = (pucChipID[0] << 24) | (pucChipID[1] << 16) | (pucChipID[2] << 8) | pucChipID[3];
		sprintf_s((char*)aucChipID, sizeof(aucChipID), "%02x%02x%02x%02x", pucChipID[0], pucChipID[1], pucChipID[2], pucChipID[3]);
		LittleToBig(aucChipID, sizeof(aucChipID));

		if (memcmp(m_acLastChipID, aucChipID, 4) != 0)
		{
			if (PEFInit((char*)aucChipID, strlen((char*)aucChipID)) == 0)
			{
				break;
			}
		}

		if (ParsePEFData(aucChipID, unChipID) == 0)
		{
			break;
		}

		/*if (ParseIFCPData(aucChipID, unChipID) == 0)
		{
			break;
		}*/

		pos = 0;
		switch (eDataType)
		{
		case E_PEF:
			unIndex = GetPEFData(&pBuffer[2], unLen - 2);
			if (unIndex == 0)
			{
				return unRet;
			}

			pBuffer[pos] = (unIndex >> 8) & 0xff;
			pBuffer[pos + 1] = unIndex & 0xff;

			break;
		case E_IFCP:
			unIndex = GetIFCPData(&pBuffer[2], unLen - 2);
			if (unIndex == 0)
			{
				break;
			}

			pBuffer[pos] = (unIndex >> 8) & 0xff;
			pBuffer[pos + 1] = unIndex & 0xff;
			break;
		default:
			break;
		}

		pos += (unIndex + 2);
		unRet = pos;
	} while (0);

	return unRet;
}

unsigned int PEF::PEFInit(char *pcChipID, int nLen)
{
	unsigned int unRet = 0;

	do
	{
		if (GetFileName(pcChipID, nLen) == 0)
		{
			break;
		}

		/*if (GetSTHFileInfo() == 0)
		{
			break;
		}

		if (GetFileInfo(m_acIFCPFileName, m_stSTHFile) == 0)
		{
			break;
		}*/

		if (GetFileInfo(m_acPEFFileName, m_stPEFFile) == 0)
		{
			break;
		}

		unRet = 1;
	} while (0);

	if (unRet == 0)
	{
		memset(m_acLastChipID, 0, sizeof(m_acLastChipID));
	}

	return unRet;
}

unsigned int PEF::ParsePEFData(unsigned char *pucChipID, unsigned int unChipID)
{
	unsigned int unRet = 0;
	unsigned int unIndex = 0;
	char acPEFData[256] = { 0 };
	char acStr[1024] = { 0 };
	FILE *pFILE = NULL;
	char *pTemp = NULL;

	do
	{
		fopen_s(&pFILE, m_acPEFFileName, "rb");
		if (pFILE == NULL)
		{
			break;
		}

		if (unChipID < m_stPEFFile.unFirstChip)
		{
			break;
		}
		unIndex = unChipID - m_stPEFFile.unFirstChip;
		fseek(pFILE, m_stPEFFile.unFirstPos + m_stPEFFile.unItemSize * unIndex, SEEK_SET);
		if (fread_s(acStr, m_stPEFFile.unItemSize, sizeof(char), m_stPEFFile.unItemSize, pFILE) == 0)
		{
			break;
		}

		pTemp = strstr(acStr, (char*)pucChipID);
		if (pTemp == NULL)
		{
			break;
		}

		if (GetFileItem(pTemp, strlen(pTemp), acPEFData, sizeof(acPEFData), "<PERDATA>", "</PERDATA>") == 0)
		{
			break;
		}

		m_unPEFDataLen = IRDCCA_Char2Hex((unsigned char*)acPEFData, strlen(acPEFData), m_ucPEFHexData);
		if (m_unPEFDataLen == 0)
		{
			break;
		}

		unRet = 1;
	} while (0);

	if (pFILE != NULL)
	{
		fclose(pFILE);
	}

	return unRet;
}

unsigned int PEF::GetPEFData(unsigned char *pucBuffer, unsigned int unLen)
{
	unsigned int unRet = 0;

	if (pucBuffer != NULL || unLen >= m_unPEFDataLen)
	{
		memcpy(pucBuffer, m_ucPEFHexData, m_unPEFDataLen);
		unRet = m_unPEFDataLen;
	}

	return unRet;
}

unsigned int PEF::ParseIFCPData(unsigned char *pucChipID, unsigned int unChipID)
{
	unsigned int unRet = 0;
	unsigned int unIndex = 0;
	char *pTemp = NULL;
	char acStr[1024] = { 0 };
	char acActMsgData[256] = { 0 };

	FILE *pFILE = NULL;

	return 0;

	do
	{
		fopen_s(&pFILE, m_acIFCPFileName, "rb");
		if (pFILE == NULL)
		{
			break;
		}

		if (unChipID < m_stSTHFile.unFirstChip)
		{
			break;
		}
		unIndex = unChipID - m_stSTHFile.unFirstChip;
		fseek(pFILE, m_stSTHFile.unFirstPos + m_stSTHFile.unItemSize * unIndex, SEEK_SET);
		if (fread_s(acStr, m_stSTHFile.unItemSize, sizeof(char), m_stSTHFile.unItemSize, pFILE) == 0)
		{
			break;
		}

		pTemp = strstr(acStr, (char*)pucChipID);
		if (pTemp == NULL)
		{
			break;
		}

		if (GetFileItem(pTemp, strlen(pTemp), acActMsgData, sizeof(acActMsgData), "<ACT_MSG>", "</ACT_MSG>") == 0)
		{
			break;
		}

		if (ConstructIFCPData((unsigned char*)acActMsgData, strlen(acActMsgData)) == 0)
		{
			break;
		}

		unRet = 1;
	} while (0);

	if (pFILE != NULL)
	{
		fclose(pFILE);
	}

	return unRet;
}

unsigned int PEF::ConstructIFCPData(unsigned char *pucActMeg, unsigned int unLen)
{
	int pos = 0;
	unsigned int unRet = 0;
	unsigned int unActMsgLen = 0;
	unsigned int unImageLen = 0;
	unsigned int unDataLen = 0;
	unsigned int payloadLength = 0;
	unsigned int unCrc = 0;

	do
	{
		if (unLen / 2 + strlen((char*)m_ucIFCP_Image) / 2 + IFCP_PS_HEADER_BINARY_LEN + IFCP_PS_IMAGE_HEADER_BINARY_LEN > sizeof(m_ucIFCPHexData))
		{
			break;
		}
		pos = 0;
		/*(4B) CRC32_H, Update header CRC later. */
		pos += 4;

		/*(1B) Version: currently fixed to 1 */
		m_ucIFCPHexData[pos++] = 1;

		/*(3B) Magic Number: "CCA" */
		m_ucIFCPHexData[pos++] = 'C';
		m_ucIFCPHexData[pos++] = 'C';
		m_ucIFCPHexData[pos++] = 'A';

		/*(4B) Length, Update this payload length later. */
		pos += 4;

		/*(4B) CRC32_D, Update IFCP Data CRC later. */
		pos += 4;

		/* --------Payload-------- */
		/*(4B) Version: currently fixed to 1 */
		m_ucIFCPHexData[pos++] = 1;

		/*(15B) Reserved */
		pos += 15;

		/*(2B) Image Version */
		m_ucIFCPHexData[pos++] = (g_ImageVersion >> 8) & 0xff;
		m_ucIFCPHexData[pos++] = g_ImageVersion & 0xff;

		/*(1B) Flag1 */
		m_ucIFCPHexData[pos] = 0;
		if (TRUE == g_IsTestImage)
		{
			m_ucIFCPHexData[pos] |= 0x01;
		}
		pos++;

		/*(7B) Reserved */
		pos += 7;

		/*(2B) AM length */
		/* AM */
		unActMsgLen = IRDCCA_Char2Hex(pucActMeg, unLen, &m_ucIFCPHexData[pos + 2]);
		m_ucIFCPHexData[pos++] = (unActMsgLen >> 8) & 0xff;
		m_ucIFCPHexData[pos++] = unActMsgLen & 0xff;
		pos += unActMsgLen;

		/*(4B) Image length */
		/* Image */
		unImageLen = IRDCCA_Char2Hex(m_ucIFCP_Image, strlen((char*)m_ucIFCP_Image), &m_ucIFCPHexData[pos + 4]);
		m_ucIFCPHexData[pos++] = (unImageLen >> 24) & 0xff;
		m_ucIFCPHexData[pos++] = (unImageLen >> 16) & 0xff;
		m_ucIFCPHexData[pos++] = (unImageLen >> 8) & 0xff;
		m_ucIFCPHexData[pos++] = unImageLen & 0xff;
		pos += unImageLen;

		unDataLen = pos;
		m_unIFCPDataLen = unDataLen;
		payloadLength = unDataLen - IFCP_PS_HEADER_BINARY_LEN;

		//crc head
		pos = 8;

		m_ucIFCPHexData[pos++] = (payloadLength >> 24) & 0xff;
		m_ucIFCPHexData[pos++] = (payloadLength >> 16) & 0xff;
		m_ucIFCPHexData[pos++] = (payloadLength >> 8) & 0xff;
		m_ucIFCPHexData[pos++] = payloadLength & 0xff;

		unCrc = GetCrc32(&m_ucIFCPHexData[IFCP_PS_HEADER_BINARY_LEN], payloadLength);

		m_ucIFCPHexData[pos++] = (unCrc >> 24) & 0xff;
		m_ucIFCPHexData[pos++] = (unCrc >> 16) & 0xff;
		m_ucIFCPHexData[pos++] = (unCrc >> 8) & 0xff;
		m_ucIFCPHexData[pos++] = unCrc & 0xff;

		unCrc = GetCrc32(&m_ucIFCPHexData[4], IFCP_PS_HEADER_BINARY_LEN - 4);
		pos = 0;
		m_ucIFCPHexData[pos++] = (unCrc >> 24) & 0xff;
		m_ucIFCPHexData[pos++] = (unCrc >> 16) & 0xff;
		m_ucIFCPHexData[pos++] = (unCrc >> 8) & 0xff;
		m_ucIFCPHexData[pos++] = unCrc & 0xff;
		//m_ucIFCPHexData

		unRet = 1;
	} while (0);

	return unRet;
}

unsigned int PEF::GetIFCPData(unsigned char *pucBuffer, unsigned int unLen)
{
	unsigned int unRet = 0;

	if (pucBuffer != NULL && unLen >= m_unIFCPDataLen)
	{
		memcpy(pucBuffer, m_ucIFCPHexData, m_unIFCPDataLen);
		unRet = m_unIFCPDataLen;
	}

	return unRet;
}

unsigned int PEF::IRDCCA_Char2Hex(const unsigned char* pucSource, unsigned int nlength, unsigned char *pucDes)
{
	unsigned int ii = 0;
	unsigned int jj = 0;

	if ((NULL == pucSource) || (NULL == pucDes))
	{
		return 0;
	}

	for (ii = 0; ii<nlength; ii += 2)
	{
		if (ii + 1<nlength)
		{
			pucDes[jj++] = (ChartoNum(pucSource[ii]) << 4) | ChartoNum(pucSource[ii + 1]);
		}
		else
		{
			pucDes[jj++] = ChartoNum(pucSource[ii]);
			break;
		}
	}
	return jj;
}

unsigned char PEF::ChartoNum(unsigned char ch)
{
	char base = 0;;

	if (ch >= 0x61)
	{
		base = 0x61 - 10;
	}
	else if (ch >= 0x41)
	{
		base = 0x41 - 10;
	}
	else if (ch >= 0x30)
	{
		base = 0x30;
	}

	return (ch - base);
}

unsigned int PEF::LittleToBig(unsigned char *pBuffer, unsigned int unLen)
{
	int i = 0;
	unsigned int unRet = 1;

	for (i = 0; i < unLen; i++)
	{
		if (pBuffer[i] >= 'a' && pBuffer[i] <= 'f')
		{
			pBuffer[i] = pBuffer[i] - ('a' - 'A');
		}
	}

	return unRet;
}

unsigned int PEF::GetFileInfo(char *pFileName, File_Info_S & stFileInfo)
{
	FILE *pFILE = NULL;
	char aucStr[1024] = { 0 };
	unsigned int unRet = 0;
	unsigned int unCurPos = 0;
	char *pTemp = NULL;
	unsigned char aucChipID[10] = { 0 };

	do
	{
		fopen_s(&pFILE, pFileName, "rb");
		if (pFILE == NULL)
		{
			break;
		}

		while ((pTemp == NULL) && (!feof(pFILE)))
		{
			if (fgets(aucStr, sizeof(aucStr), pFILE) == 0)
			{
				break;
			}
			pTemp = strstr(aucStr, "<CHIP>");
		}

		if (pTemp == NULL)
		{
			break;
		}
		stFileInfo.unFirstPos = ftell(pFILE) - strlen(aucStr);

		pTemp = NULL;
		while ((pTemp == NULL) && (!feof(pFILE)))
		{
			if (fgets(aucStr, sizeof(aucStr), pFILE) == 0)
			{
				break;
			}
			pTemp = strstr(aucStr, "<SERIAL>");
		}
		if (pTemp == NULL)
		{
			break;
		}
		pTemp += strlen("<SERIAL>");
		int i = 0;
		while (pTemp[i] != '<' && i < sizeof(aucChipID))
		{
			aucChipID[i] = pTemp[i];
			i++;
		}
		stFileInfo.unFirstChip = 0;
		GetChipID(aucChipID, strlen((char*)aucChipID), stFileInfo.unFirstChip);

		pTemp = NULL;
		while ((pTemp == NULL) && (!feof(pFILE)))
		{
			if (fgets(aucStr, sizeof(aucStr), pFILE) == 0)
			{
				break;
			}
			pTemp = strstr(aucStr, "<CHIP>");
		}

		if (pTemp == NULL)
		{
			break;
		}
		unCurPos = ftell(pFILE) - strlen(aucStr);
		stFileInfo.unItemSize = unCurPos - stFileInfo.unFirstPos;

		unRet = 1;
	} while (0);

	if (pFILE != NULL)
	{
		fclose(pFILE);
	}

	return unRet;
}

unsigned int PEF::GetSTHFileInfo()
{
	FILE *pFILE = NULL;
	char cVersion[8] = { 0 };
	char cBuffer[16] = { 0 };
	char acIFCPTag[128] = { 0 };
	char acStr[128 * 1024] = { 0 };
	unsigned int unRet = 0;

	do
	{
		fopen_s(&pFILE, m_acIFCPFileName, "rb");
		if (pFILE == NULL)
		{
			break;
		}

		if (fread_s(acStr, sizeof(acStr), sizeof(char), sizeof(acStr), pFILE) == 0)
		{
			break;
		}

		if (GetFileItem(acStr, sizeof(acStr), (char*)m_ucIPCP_cwodd, sizeof(m_ucIPCP_cwodd), "<CW_ODD>", "</CW_ODD>") == 0)
		{
			break;
		}

		if (GetFileItem(acStr, sizeof(acStr), (char*)m_unIFCP_cweven, sizeof(m_unIFCP_cweven), "<CW_EVEN>", "</CW_EVEN>") == 0)
		{
			break;
		}

		if (GetFileItem(acStr, sizeof(acStr), (char*)m_ucIFCP_k2, sizeof(m_ucIFCP_k2), "<K2>", "</K2>") == 0)
		{
			break;
		}

		if (GetFileItem(acStr, sizeof(acStr), cVersion, sizeof(cVersion), "<IFCP_IMAGE image_version=\"", "\" test_image=\"") == 0)
		{
			break;
		}
		if (GetVersion(cVersion, strlen(cVersion)) == 0)
		{
			break;
		}
		//printf("cVersion:%s\n", cVersion);
		sprintf_s(acIFCPTag, sizeof(acIFCPTag), "<IFCP_IMAGE image_version=\"%s\" test_image=\"", cVersion);

		if (GetFileItem(acStr, sizeof(acStr), cBuffer, sizeof(cBuffer), acIFCPTag, "\">") == 0)
		{
			break;
		}
		if (GetIsTestImage(cBuffer, strlen(cBuffer)) == 0)
		{
			break;
		}
		sprintf_s(acIFCPTag, sizeof(acIFCPTag), "<IFCP_IMAGE image_version=\"%s\" test_image=\"%s\">", cVersion, cBuffer);

		if (GetFileItem(acStr, sizeof(acStr), (char*)m_ucIFCP_Image, sizeof(m_ucIFCP_Image), acIFCPTag, "</IFCP_IMAGE>") == 0)
		{
			break;
		}

		unRet = 1;
	} while (0);

	if (pFILE != NULL)
	{
		fclose(pFILE);
	}

	return unRet;
}

unsigned int PEF::GetFileItem(char *pBuffer, int nBufLen, char *pDest, int nDestLen, char *pStartData, char *pEndData)
{
	unsigned int unRet = 0;
	char *pStart = NULL;
	char *pEnd = NULL;
	int i = 0;

	do
	{
		pStart = strstr(pBuffer, pStartData);
		if (pStart == NULL)
		{
			break;
		}

		pEnd = strstr(pStart, pEndData);
		if (pEnd == NULL)
		{
			break;
		}

		pStart += strlen(pStartData);
		while (((pStart + i) != pEnd) && (i < nDestLen))
		{
			pDest[i] = pStart[i];
			i++;
		}

		if ((pStart + i) != pEnd)
		{
			break;
		}

		unRet = 1;
	} while (0);

	return unRet;
}

unsigned int PEF::GetChipID(unsigned char *pBuffer, unsigned int unLen, unsigned int & unChipID)
{
	int i = 0;
	unsigned int unRet = 0;

	for (i = 0; i < unLen; i++)
	{
		if (pBuffer[i] >= '0' && pBuffer[i] <= '9')
		{
			unChipID = unChipID * 16 + pBuffer[i] - '0';
		}
		else if (pBuffer[i] >= 'a' && pBuffer[i] <= 'f')
		{
			unChipID = unChipID * 16 + pBuffer[i] - 'a' + 10;
		}
		else if (pBuffer[i] >= 'A' && pBuffer[i] <= 'F')
		{
			unChipID = unChipID * 16 + pBuffer[i] - 'A' + 10;
		}
		else
		{
			break;
		}
	}

	if (i == unLen)
	{
		unRet = 1;
	}

	return unRet;
}

unsigned int PEF::GetCrc32(const unsigned char *pDataBuffer, unsigned int dataLength)
{
	unsigned int crcResult = 0xffffffff;

	while (dataLength--)
	{
		crcResult = crc32Table[((crcResult >> 24) ^ *pDataBuffer++) & 0xFFL] ^ (crcResult << 8);
	}

	return crcResult;
}

void PEF::ClearAllData(void)
{
	m_unPEFDataLen = 0;
	memset(m_ucPEFHexData, 0, sizeof(m_ucPEFHexData));

	m_unIFCPDataLen = 0;
	memset(m_ucIFCPHexData, 0, sizeof(m_ucIFCPHexData));
}

unsigned int PEF::GetVersion(char *pacBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pacBuffer != NULL && nLen > 0)
	{
		g_ImageVersion = 0;
		for (int ii = 0; ii < nLen; ii++)
		{
			g_ImageVersion = ChartoNum(pacBuffer[ii]) + g_ImageVersion * 10;
		}
		unRet = 1;
	}

	return unRet;
}

unsigned int PEF::GetIsTestImage(char *pacBuffer, int nLen)
{
	unsigned int unRet = 0;

	if (pacBuffer != NULL && nLen > 0)
	{
		if (memcmp(pacBuffer, "false", strlen("false")) == 0 || memcmp(pacBuffer, "FALSE", strlen("FALSE")) == 0)
		{
			g_IsTestImage = FALSE;
		}
		else
		{
			g_IsTestImage = TRUE;
		}

		unRet = 1;
	}

	return unRet;
}

unsigned int PEF::GetFileName(char *cChipID, int nLen)
{
	char cName[8] = { 0 };
	unsigned int unRet = 0;

	if (cChipID != NULL && nLen > 0)
	{
		LittleToBig((unsigned char*)cChipID, sizeof(cChipID));

		memset(m_acPEFFileName, 0, sizeof(m_acPEFFileName));
		memset(m_acIFCPFileName, 0, sizeof(m_acIFCPFileName));

		memset(m_acLastChipID, 0, sizeof(m_acLastChipID));
		memcpy(m_acLastChipID, cChipID, 4);

		sprintf_s(m_acPEFFileName, sizeof(m_acPEFFileName), "Source/%s_%s", m_acLastChipID, PEF_FILE);
		sprintf_s(m_acIFCPFileName, sizeof(m_acIFCPFileName), "Source/%s_%s", m_acLastChipID, IFCP_FILE);

		unRet = 1;
	}

	return unRet;
}