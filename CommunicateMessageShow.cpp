#include "CommunicateMessageShow.h"
#include "StartRun/UpgradeCommand.h"
#include <string.h>
#include <stdio.h>

CommunicateMessageShow::CommunicateMessageShow(unsigned int unConnType):
m_unConnType(unConnType)
{
}

CommunicateMessageShow::~CommunicateMessageShow()
{
}

void CommunicateMessageShow::SetConnType(unsigned int unConnType)
{
	m_unConnType = unConnType;
}

int CommunicateMessageShow::ShowSTBMessage(int nCommand, char *pBuffer, int nLen)
{
	int nRet = 1;

	switch (nCommand)
	{
	case E_UPGRADE_COMMAND_STB_SEND_UPGRADE_COMMAND:
		sprintf_s(pBuffer, nLen, "STB�����պ�");
		break;

	case E_UPGRADE_COMMAND_STB_REQ_SN:
		sprintf_s(pBuffer, nLen, "STB������¼����");
		break;

	case E_UPGRADE_COMMAND_STB_REQ_HDCP:
		break;

	case E_UPGRADE_COMMAND_STB_REQ_MAC:
		break;

	case E_UPGRADE_COMMAND_STB_REQ_PEF:
		break;

	case E_UPGRADE_COMMAND_STB_REQ_IFCP_SIZE:
		sprintf_s(pBuffer, nLen, "STB����IFCP");
		break;

	//case E_UPGRADE_COMMAND_STB_REQ_IFCP:
	//	sprintf_s(pBuffer, nLen, "STB����IFCP����");
	//	break;

	case E_UPGRADE_COMMAND_STB_SEND_WAIT_SEND_UPGRADE_RESULT:
		sprintf_s(pBuffer, nLen, "�ȴ�PC����Return Data");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_UPGRADE_RESULT:
		sprintf_s(pBuffer, nLen, "STB����Return Data");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_INSERT_DB:
		sprintf_s(pBuffer, nLen, "STB�պ����,�����ϴ����ݵ����ݿ�");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_WAIT_VERIFY:
		sprintf_s(pBuffer, nLen, "STB�ȴ�У��");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_VERIFY_DATA:
		sprintf_s(pBuffer, nLen, "STB����У������");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_VERIFY_FINISHED:
		sprintf_s(pBuffer, nLen, "STBУ�ųɹ����뿪ʼ��һ̨");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_ERASE_RESULT:
		break;

	case E_UPGRADE_COMMAND_STB_SEND_CHIPID_ERROR:
		break;

	case E_UPGRADE_COMMAND_STB_SEND_PERSONALIZE_COMMAND:
		sprintf_s(pBuffer, nLen, "STB������Ի�����KEY");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_ENCRYT_RESULT:
		sprintf_s(pBuffer, nLen, "STB������Ի����ݳ���");
		break;

	case E_UPGRADE_COMMAND_STB_REQUEST_VSCDATA:
		sprintf_s(pBuffer, nLen, "STB������Ի�");
		break;

	case E_UPGRADE_COMMAND_STB_RECV_VSCDATA:
		sprintf_s(pBuffer, nLen, "STB�յ����Ի�����");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_PERSONALIZE_RESULT:
		sprintf_s(pBuffer, nLen, "STB�յ����Ի�����");
		break;

	default:
	{
		if (nCommand >= E_UPGRADE_COMMAND_STB_REQ_IFCP && nCommand <= E_UPGRADE_COMMAND_STB_REQ_IFCP_MAX)
		{
			for (unsigned int ii = E_UPGRADE_COMMAND_STB_REQ_IFCP; ii < E_UPGRADE_COMMAND_STB_REQ_IFCP_MAX; ii++)
			{
				if (ii == nCommand)
				{
					sprintf_s(pBuffer, nLen, "STB����IFCP���ݵ�%d�ֶ�", nCommand - E_UPGRADE_COMMAND_STB_REQ_IFCP + 1);
					break;
				}
			}
			nRet = 1;
		}
		else
		{
			sprintf_s(pBuffer, nLen, "�ȴ�STB����");
			nRet = 0;
		}
	}
		break;
	}

	return nRet;
}

int CommunicateMessageShow::ShowErrorMessage(int nCommand, char *pBuffer, int nLen)
{
	int nRet = 0;
	int nType = ((nCommand >> 16) & 0xffff);
	int nMsg = (nCommand & 0xffff);

	switch (nType)
	{
	case 1:
		ShowUpgradeStatus(nMsg, pBuffer, nLen);
		break;
	case 2:
		ShowReturnFileStatus(nMsg, pBuffer, nLen);
		break;
	case 3:
		ShowInsertDBStatus(nMsg, pBuffer, nLen);
		break;
	//case 4:
	//	ShowConaxPersonalizeStatus(nMsg, pBuffer, nLen);
	//	break;

	case 5:
		ShowEraseStatus(nMsg, pBuffer, nLen);
		break;

	case 6:
		ShowConnectStatus(nMsg, pBuffer, nLen);
		break;

	case 7:
		ShowConaxPersonalizeStatus(nMsg, pBuffer, nLen);
		break;

	case 8:
		ShowSTBStatus(nMsg, pBuffer, nLen);
		break;

	case 9:
		ShowVerifyStatus(nMsg, pBuffer, nLen);
		break;

	default:
		break;
	}

	return nRet;
}

void CommunicateMessageShow::ShowVerifyStatus(unsigned int unVerifyStatus, char *pBuffer, int nLen)
{
	if (unVerifyStatus == 0)
	{
		sprintf_s(pBuffer, nLen, "���ʧ��");
	}
	else if (unVerifyStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "������");
	}
	else if (unVerifyStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "��ų�ʱ");
	}
	else if (unVerifyStatus == 3)
	{
		sprintf_s(pBuffer, nLen, "��ɨ������");
	}
	else if (unVerifyStatus == 4)
	{
		sprintf_s(pBuffer, nLen, "������Ž��ʧ��");
	}
	else if (unVerifyStatus == 5)
	{
		sprintf_s(pBuffer, nLen, "��ųɹ�,��������һ��");
	}
	else if (unVerifyStatus == 6)
	{
		sprintf_s(pBuffer, nLen, "����У������ʧ��");
	}
}

void CommunicateMessageShow::ShowUpgradeStatus(unsigned int unUpgradeStatus, char *pBuffer, int nLen)
{
	switch (unUpgradeStatus)
	{
	case E_UPGRADE_FAIL:
		sprintf_s(pBuffer, nLen, "��������ʧ��");
		break;

	case E_UPGRADE_SUCCESS:
		sprintf_s(pBuffer, nLen, "�������ݳɹ�");
		break;

	case E_UPGRADE_START:
		sprintf_s(pBuffer, nLen, "��ʼ�պ����������������");
		break;

	case E_UPGRADE_WAIT_FOR_REQ_DATA:
		sprintf_s(pBuffer, nLen, "�ȴ�STB��������");
		break;

	case E_UPGRADE_WAIT_FOR_REQ_DATA_TIMEOUT:
		sprintf_s(pBuffer, nLen, "�������ݳ�ʱ");
		break;

	case E_UPGRADE_START_SEND_DATA:
		sprintf_s(pBuffer, nLen, "��ʼ��������");
		break;

	case E_UPGRADE_WAIT_FOR_STB_SAVE_UPGRADE_RESULT_TIMEOUT:
		sprintf_s(pBuffer, nLen, "�ȴ�STB����SN��ʱ");
		break;

	case E_UPGRADE_STB_SEND_ERROR_DATA:
		sprintf_s(pBuffer, nLen, "STB���ݴ���");
		break;

	case E_UPGRADE_GET_SN_ERROR:
		sprintf_s(pBuffer, nLen, "��ȡSN����");
		break;

	case E_UPGRADE_GET_HDCPKEY_ERROR:
		sprintf_s(pBuffer, nLen, "��ȡHDCP����");
		break;

	case E_UPGRADE_GET_MAC_ERROR:
		sprintf_s(pBuffer, nLen, "��ȡMAC����");
		break;

	case E_UPGRADE_GET_PEF_ERROR:
		sprintf_s(pBuffer, nLen, "��ȡPEF/IFCP����");
		break;

	case E_UPGRADE_REQUEST_DATA_FAIL:
		sprintf_s(pBuffer, nLen, "�����������������ʧ�����������������");
		break;

	case E_UPGRADE_CHIPID_ERROR:
		sprintf_s(pBuffer, nLen, "��鿴ChipID�Ƿ�ƥ������ƥ�����������");
		break;

	case E_UPGRADE_IFCP_ERROR:
		sprintf_s(pBuffer, nLen, "STB����IFCP Result����");
		break;
		;
	default:
		break;
	}
}

void CommunicateMessageShow::ShowEraseStatus(unsigned int unEraseStatus, char *pBuffer, int nLen)
{
	if (unEraseStatus == 0)
	{
		sprintf_s(pBuffer, nLen, "����ʧ��");
	}
	else if (unEraseStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "�����ɹ�");
	}
	else if (unEraseStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "�����½����պ�...");
	}
	else
	{
		sprintf_s(pBuffer, nLen, "������ʱ");
	}
}

void CommunicateMessageShow::ShowReturnFileStatus(unsigned int unReturnStatus, char *pBuffer, int nLen)
{
	if (unReturnStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "����returnfile�ɹ�");
	}
	else if (unReturnStatus == 0)
	{
		sprintf_s(pBuffer, nLen, "����returnfileʧ��");
	}
	else if (unReturnStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "��ʼ����returnfile");
	}
}

void CommunicateMessageShow::ShowInsertDBStatus(unsigned int unInsertStatus, char *pBuffer, int nLen)
{
	if (unInsertStatus == 0)
	{
		sprintf_s(pBuffer, nLen, "д���ݿ�ʧ��");
	}
	else if (unInsertStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "д���ݿ�ɹ�");
	}
	else if (unInsertStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "��ʼд���ݿ�");
	}
	else if (unInsertStatus == 3)
	{
		sprintf_s(pBuffer, nLen, "ͨѶ����");
	}
	else if (unInsertStatus == 4)
	{
		sprintf_s(pBuffer, nLen, "�����ӡ����ʧ��");
	}
}

void CommunicateMessageShow::ShowConnectStatus(unsigned int unRegStatus, char *pBuffer, int nLen)
{
	char acConnName[][32] = { "����", "����" };

	if (unRegStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "ע��ͻ��˳ɹ�");
	}
	else if (unRegStatus == 0)
	{
		sprintf_s(pBuffer, nLen, "ע��ͻ���ʧ��");
	}
	else if (unRegStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "����ע��ͻ���");
	}
	else if (unRegStatus == 3)
	{
		sprintf_s(pBuffer, nLen, "��%sʧ��", acConnName[m_unConnType]);
	}
	else if (unRegStatus == 4)
	{
		sprintf_s(pBuffer, nLen, "��%s�ɹ�", acConnName[m_unConnType]);
	}
}

void CommunicateMessageShow::ShowConaxPersonalizeStatus(unsigned int unPersonalizeStatus, char *pBuffer, int nLen)
{
	switch (unPersonalizeStatus)
	{
	case E_PERSONALIZE_FAIL:
		sprintf_s(pBuffer, nLen, "���Ի�ʧ��");
		break;

	case E_PERSONALIZE_SUCCESS:
		sprintf_s(pBuffer, nLen, "���Ի��ɹ�");
		break;

	case E_PERSONALIZE_START:
		sprintf_s(pBuffer, nLen, "��ʼ���Ի�");
		break;

	case E_GET_JOBID_FAIL:
		sprintf_s(pBuffer, nLen, "��ȡJOBIDʧ��,����jobfile����");
		break;

	case E_GET_ENCRYPTKEY_FAIL:
		sprintf_s(pBuffer, nLen, "��ȡ���Ի�KEYʧ��");
		break;

	case E_SEND_ENCRYPTKEY:
		sprintf_s(pBuffer, nLen, "��ʼ���͸��Ի�KEY");
		break;

	case E_SEND_ENCRYPTKEY_FAIL:
		sprintf_s(pBuffer, nLen, "���͸��Ի�KEYʧ��");
		break;

	case E_GET_VSCDATA_FAIL:
		sprintf_s(pBuffer, nLen, "��ȡVSC DATAʧ��");
		break;

	case E_SEND_VSCDATA:
		sprintf_s(pBuffer, nLen, "��ʼ����VSC DATA");
		break;

	case E_SEND_VSCDATA_FAIL:
		sprintf_s(pBuffer, nLen, "����VSC DATAʧ��");
		break;

	case E_GET_PERSONALIZE_RESULT_FAIL:
		sprintf_s(pBuffer, nLen, "��ȡ���Ի����ʧ��");
		break;

	case E_STB_REBOOT_FOR_INIT:
		sprintf_s(pBuffer, nLen, "��������STB...");
		break;

	default:
		break;
	}
}

void CommunicateMessageShow::ShowSTBStatus(unsigned int unSTBStatus, char *pBuffer, int nLen)
{
	switch (unSTBStatus)
	{
	case WAIT_UPGRADE_DATA:
		sprintf_s(pBuffer, nLen, "�ȴ��պ�");
		break;

	case UPGRADE_FINISHED:
		sprintf_s(pBuffer, nLen, "�պ���ɣ��ɽ���OTA����");
		break;

	case WAIT_SEND_RETURNFILE:
		sprintf_s(pBuffer, nLen, "�ȴ�����return file");
		break;

	case WAIT_DB_RESULT:
		sprintf_s(pBuffer, nLen, "�ȴ��ϴ����ݿ�");
		break;

	case WAIT_VERIFY:
		sprintf_s(pBuffer, nLen, "�ȴ�У��");
		break;

	case CHIPID_ERROR:
		sprintf_s(pBuffer, nLen, "CHIPID�뵱ǰоƬ��ƥ��");
		break;

	case WAIT_COMMAND:
		sprintf_s(pBuffer, nLen, "�ȴ�PC����");
		break;

	case WAIT_CONAX_PERSONALIZE:
		sprintf_s(pBuffer, nLen, "�ȴ����Ի�");
		break;

	default:
		break;
	}
}