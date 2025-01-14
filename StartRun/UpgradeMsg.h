#ifndef UPGRADEMSG_H_
#define UPGRADEMSG_H_

#include <afxwin.h> 

#define MSG_USER_MSG (WM_USER + 1001)
#define MSG_USER_MSG_START_UPGRADE (WM_USER + 1001)
#define MSG_USER_MSG_STOP_UPGRADE (WM_USER + 1002)
#define MSG_USER_MSG_UPGRADE_MSG (WM_USER + 1003)
#define MSG_USER_MSG_VERIFY_MSG (WM_USER + 1004)
#define MSG_USER_MSG_ERASE_MSG (WM_USER + 1005)
#define MSG_USER_MSG_OPEN_COM_MSG (WM_USER + 1006)
#define MSG_USER_MSG_SET_VERIFY_FOCUS (WM_USER + 1007)
#define MSG_USER_MSG_SEND_DATA_SEG (WM_USER + 1008)

#define MSG_USER_MSG_INIT_NETWORK_FAIL (WM_USER + 1010)
#define MSG_USER_MSG_RECEVIE_RETURNFILE (WM_USER + 1011)
#define MSG_USER_MSG_INSERT_DATA_INTO_DB (WM_USER + 1012)
#define MSG_USER_MSG_WAIT_MSG (WM_USER + 1013)
#define MSG_USER_MSG_OPEN_COM (WM_USER + 1014)
#define MSG_USER_MSG_REG_CLIENT (WM_USER + 1015)
#define MSG_USER_MSG_CONAX_PERSONALIZATION (WM_USER + 1016)
#define MSG_USER_MSG_UPGRADE_STATUS (WM_USER + 1017)

typedef enum
{
	E_UPGRADE_FAIL=0,
	E_UPGRADE_SUCCESS,
	E_UPGRADE_START,
	E_UPGRADE_WAIT_FOR_REQ_DATA,
	E_UPGRADE_WAIT_FOR_REQ_DATA_TIMEOUT,
	E_UPGRADE_START_SEND_DATA,
	E_UPGRADE_WAIT_FOR_STB_SAVE_UPGRADE_RESULT_TIMEOUT,
	E_UPGRADE_STB_SEND_ERROR_DATA,
	E_UPGRADE_GET_SN_ERROR,
	E_UPGRADE_GET_HDCPKEY_ERROR,
	E_UPGRADE_GET_MAC_ERROR,
	E_UPGRADE_GET_PEF_ERROR,
	E_UPGRADE_REQUEST_DATA_FAIL,
	E_UPGRADE_CHIPID_ERROR,
	E_UPGRADE_IFCP_ERROR,
	E_UPGRADE_NODE_ERROR
}E_UpgradeStatus;

typedef enum
{
	E_PERSONALIZE_FAIL=0,
	E_PERSONALIZE_SUCCESS,
	E_PERSONALIZE_START,
	E_GET_JOBID_FAIL,
	E_GET_ENCRYPTKEY_FAIL,
	E_SEND_ENCRYPTKEY,
	E_SEND_ENCRYPTKEY_FAIL,
	E_GET_VSCDATA_FAIL,
	E_SEND_VSCDATA,
	E_SEND_VSCDATA_FAIL,
	E_GET_PERSONALIZE_RESULT_FAIL,
	E_STB_REBOOT_FOR_INIT
}E_PersonalizeStatus;

typedef enum
{
	WAIT_UPGRADE_DATA = 0,
	UPGRADE_FINISHED,
	WAIT_SEND_RETURNFILE,
	WAIT_DB_RESULT,
	WAIT_VERIFY,
	CHIPID_ERROR,
	WAIT_COMMAND,
	WAIT_CONAX_PERSONALIZE,
	MAX_STATUS
}STB_UPGRADE_STATUS;

#endif /* UPGRADEMSG_H_ */