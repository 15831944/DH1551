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
		sprintf_s(pBuffer, nLen, "STB请求烧号");
		break;

	case E_UPGRADE_COMMAND_STB_REQ_SN:
		sprintf_s(pBuffer, nLen, "STB请求烧录数据");
		break;

	case E_UPGRADE_COMMAND_STB_REQ_HDCP:
		break;

	case E_UPGRADE_COMMAND_STB_REQ_MAC:
		break;

	case E_UPGRADE_COMMAND_STB_REQ_PEF:
		break;

	case E_UPGRADE_COMMAND_STB_REQ_IFCP_SIZE:
		sprintf_s(pBuffer, nLen, "STB请求IFCP");
		break;

	//case E_UPGRADE_COMMAND_STB_REQ_IFCP:
	//	sprintf_s(pBuffer, nLen, "STB请求IFCP数据");
	//	break;

	case E_UPGRADE_COMMAND_STB_SEND_WAIT_SEND_UPGRADE_RESULT:
		sprintf_s(pBuffer, nLen, "等待PC请求Return Data");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_UPGRADE_RESULT:
		sprintf_s(pBuffer, nLen, "STB发送Return Data");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_INSERT_DB:
		sprintf_s(pBuffer, nLen, "STB烧号完成,请求上传数据到数据库");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_WAIT_VERIFY:
		sprintf_s(pBuffer, nLen, "STB等待校号");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_VERIFY_DATA:
		sprintf_s(pBuffer, nLen, "STB发送校验数据");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_VERIFY_FINISHED:
		sprintf_s(pBuffer, nLen, "STB校号成功，请开始下一台");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_ERASE_RESULT:
		break;

	case E_UPGRADE_COMMAND_STB_SEND_CHIPID_ERROR:
		break;

	case E_UPGRADE_COMMAND_STB_SEND_PERSONALIZE_COMMAND:
		sprintf_s(pBuffer, nLen, "STB请求个性化加密KEY");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_ENCRYT_RESULT:
		sprintf_s(pBuffer, nLen, "STB请求个性化数据长度");
		break;

	case E_UPGRADE_COMMAND_STB_REQUEST_VSCDATA:
		sprintf_s(pBuffer, nLen, "STB请求个性化");
		break;

	case E_UPGRADE_COMMAND_STB_RECV_VSCDATA:
		sprintf_s(pBuffer, nLen, "STB收到个性化数据");
		break;

	case E_UPGRADE_COMMAND_STB_SEND_PERSONALIZE_RESULT:
		sprintf_s(pBuffer, nLen, "STB收到个性化数据");
		break;

	default:
	{
		if (nCommand >= E_UPGRADE_COMMAND_STB_REQ_IFCP && nCommand <= E_UPGRADE_COMMAND_STB_REQ_IFCP_MAX)
		{
			for (unsigned int ii = E_UPGRADE_COMMAND_STB_REQ_IFCP; ii < E_UPGRADE_COMMAND_STB_REQ_IFCP_MAX; ii++)
			{
				if (ii == nCommand)
				{
					sprintf_s(pBuffer, nLen, "STB请求IFCP数据第%d分段", nCommand - E_UPGRADE_COMMAND_STB_REQ_IFCP + 1);
					break;
				}
			}
			nRet = 1;
		}
		else
		{
			sprintf_s(pBuffer, nLen, "等待STB命令");
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
		sprintf_s(pBuffer, nLen, "验号失败");
	}
	else if (unVerifyStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "验号完成");
	}
	else if (unVerifyStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "验号超时");
	}
	else if (unVerifyStatus == 3)
	{
		sprintf_s(pBuffer, nLen, "请扫描条码");
	}
	else if (unVerifyStatus == 4)
	{
		sprintf_s(pBuffer, nLen, "返回验号结果失败");
	}
	else if (unVerifyStatus == 5)
	{
		sprintf_s(pBuffer, nLen, "验号成功,请升级下一个");
	}
	else if (unVerifyStatus == 6)
	{
		sprintf_s(pBuffer, nLen, "返回校验数据失败");
	}
}

void CommunicateMessageShow::ShowUpgradeStatus(unsigned int unUpgradeStatus, char *pBuffer, int nLen)
{
	switch (unUpgradeStatus)
	{
	case E_UPGRADE_FAIL:
		sprintf_s(pBuffer, nLen, "发送数据失败");
		break;

	case E_UPGRADE_SUCCESS:
		sprintf_s(pBuffer, nLen, "发送数据成功");
		break;

	case E_UPGRADE_START:
		sprintf_s(pBuffer, nLen, "开始烧号向服务器请求数据");
		break;

	case E_UPGRADE_WAIT_FOR_REQ_DATA:
		sprintf_s(pBuffer, nLen, "等待STB请求数据");
		break;

	case E_UPGRADE_WAIT_FOR_REQ_DATA_TIMEOUT:
		sprintf_s(pBuffer, nLen, "请求数据超时");
		break;

	case E_UPGRADE_START_SEND_DATA:
		sprintf_s(pBuffer, nLen, "开始发送数据");
		break;

	case E_UPGRADE_WAIT_FOR_STB_SAVE_UPGRADE_RESULT_TIMEOUT:
		sprintf_s(pBuffer, nLen, "等待STB保存SN超时");
		break;

	case E_UPGRADE_STB_SEND_ERROR_DATA:
		sprintf_s(pBuffer, nLen, "STB数据错误");
		break;

	case E_UPGRADE_GET_SN_ERROR:
		sprintf_s(pBuffer, nLen, "获取SN错误");
		break;

	case E_UPGRADE_GET_HDCPKEY_ERROR:
		sprintf_s(pBuffer, nLen, "获取HDCP错误");
		break;

	case E_UPGRADE_GET_MAC_ERROR:
		sprintf_s(pBuffer, nLen, "获取MAC错误");
		break;

	case E_UPGRADE_GET_PEF_ERROR:
		sprintf_s(pBuffer, nLen, "获取PEF/IFCP错误");
		break;

	case E_UPGRADE_REQUEST_DATA_FAIL:
		sprintf_s(pBuffer, nLen, "请求服务器发送数据失败请检查服务器及网络");
		break;

	case E_UPGRADE_CHIPID_ERROR:
		sprintf_s(pBuffer, nLen, "请查看ChipID是否匹配若不匹配请擦除数据");
		break;

	case E_UPGRADE_IFCP_ERROR:
		sprintf_s(pBuffer, nLen, "STB发送IFCP Result错误");
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
		sprintf_s(pBuffer, nLen, "擦除失败");
	}
	else if (unEraseStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "擦除成功");
	}
	else if (unEraseStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "请重新进行烧号...");
	}
	else
	{
		sprintf_s(pBuffer, nLen, "擦除超时");
	}
}

void CommunicateMessageShow::ShowReturnFileStatus(unsigned int unReturnStatus, char *pBuffer, int nLen)
{
	if (unReturnStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "接收returnfile成功");
	}
	else if (unReturnStatus == 0)
	{
		sprintf_s(pBuffer, nLen, "接收returnfile失败");
	}
	else if (unReturnStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "开始请求returnfile");
	}
}

void CommunicateMessageShow::ShowInsertDBStatus(unsigned int unInsertStatus, char *pBuffer, int nLen)
{
	if (unInsertStatus == 0)
	{
		sprintf_s(pBuffer, nLen, "写数据库失败");
	}
	else if (unInsertStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "写数据库成功");
	}
	else if (unInsertStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "开始写数据库");
	}
	else if (unInsertStatus == 3)
	{
		sprintf_s(pBuffer, nLen, "通讯错误");
	}
	else if (unInsertStatus == 4)
	{
		sprintf_s(pBuffer, nLen, "构造打印数据失败");
	}
}

void CommunicateMessageShow::ShowConnectStatus(unsigned int unRegStatus, char *pBuffer, int nLen)
{
	char acConnName[][32] = { "串口", "网口" };

	if (unRegStatus == 1)
	{
		sprintf_s(pBuffer, nLen, "注册客户端成功");
	}
	else if (unRegStatus == 0)
	{
		sprintf_s(pBuffer, nLen, "注册客户端失败");
	}
	else if (unRegStatus == 2)
	{
		sprintf_s(pBuffer, nLen, "正在注册客户端");
	}
	else if (unRegStatus == 3)
	{
		sprintf_s(pBuffer, nLen, "打开%s失败", acConnName[m_unConnType]);
	}
	else if (unRegStatus == 4)
	{
		sprintf_s(pBuffer, nLen, "打开%s成功", acConnName[m_unConnType]);
	}
}

void CommunicateMessageShow::ShowConaxPersonalizeStatus(unsigned int unPersonalizeStatus, char *pBuffer, int nLen)
{
	switch (unPersonalizeStatus)
	{
	case E_PERSONALIZE_FAIL:
		sprintf_s(pBuffer, nLen, "个性化失败");
		break;

	case E_PERSONALIZE_SUCCESS:
		sprintf_s(pBuffer, nLen, "个性化成功");
		break;

	case E_PERSONALIZE_START:
		sprintf_s(pBuffer, nLen, "开始个性化");
		break;

	case E_GET_JOBID_FAIL:
		sprintf_s(pBuffer, nLen, "获取JOBID失败,请检查jobfile配置");
		break;

	case E_GET_ENCRYPTKEY_FAIL:
		sprintf_s(pBuffer, nLen, "获取个性化KEY失败");
		break;

	case E_SEND_ENCRYPTKEY:
		sprintf_s(pBuffer, nLen, "开始发送个性化KEY");
		break;

	case E_SEND_ENCRYPTKEY_FAIL:
		sprintf_s(pBuffer, nLen, "发送个性化KEY失败");
		break;

	case E_GET_VSCDATA_FAIL:
		sprintf_s(pBuffer, nLen, "获取VSC DATA失败");
		break;

	case E_SEND_VSCDATA:
		sprintf_s(pBuffer, nLen, "开始发送VSC DATA");
		break;

	case E_SEND_VSCDATA_FAIL:
		sprintf_s(pBuffer, nLen, "发送VSC DATA失败");
		break;

	case E_GET_PERSONALIZE_RESULT_FAIL:
		sprintf_s(pBuffer, nLen, "获取个性化结果失败");
		break;

	case E_STB_REBOOT_FOR_INIT:
		sprintf_s(pBuffer, nLen, "正在重启STB...");
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
		sprintf_s(pBuffer, nLen, "等待烧号");
		break;

	case UPGRADE_FINISHED:
		sprintf_s(pBuffer, nLen, "烧号完成，可进行OTA升级");
		break;

	case WAIT_SEND_RETURNFILE:
		sprintf_s(pBuffer, nLen, "等待请求return file");
		break;

	case WAIT_DB_RESULT:
		sprintf_s(pBuffer, nLen, "等待上传数据库");
		break;

	case WAIT_VERIFY:
		sprintf_s(pBuffer, nLen, "等待校验");
		break;

	case CHIPID_ERROR:
		sprintf_s(pBuffer, nLen, "CHIPID与当前芯片不匹配");
		break;

	case WAIT_COMMAND:
		sprintf_s(pBuffer, nLen, "等待PC命令");
		break;

	case WAIT_CONAX_PERSONALIZE:
		sprintf_s(pBuffer, nLen, "等待个性化");
		break;

	default:
		break;
	}
}