#ifndef COMMUNICATE_MESSAGE_SHOW_H_
#define COMMUNICATE_MESSAGE_SHOW_H_

#include "StartRun/UpgradeMsg.h"

class CommunicateMessageShow
{
public:
	CommunicateMessageShow(unsigned int unConnType = 0);
	~CommunicateMessageShow();

public:
	void SetConnType(unsigned int unConnType);
	int ShowSTBMessage(int nCommand, char *pBuffer, int nLen);
	int ShowErrorMessage(int nCommand, char *pBuffer, int nLen);

private:
	void ShowVerifyStatus(unsigned int unVerifyStatus, char *pBuffer, int nLen);
	void ShowUpgradeStatus(unsigned int unUpgradeStatus, char *pBuffer, int nLen);
	void ShowEraseStatus(unsigned int unEraseStatus, char *pBuffer, int nLen);
	void ShowReturnFileStatus(unsigned int unReturnStatus, char *pBuffer, int nLen);
	void ShowInsertDBStatus(unsigned int unInsertStatus, char *pBuffer, int nLen);
	void ShowConnectStatus(unsigned int unRegStatus, char *pBuffer, int nLen);
	void ShowConaxPersonalizeStatus(unsigned int unPersonalizeStatus, char *pBuffer, int nLen);
	void ShowSTBStatus(unsigned int unSTBStatus, char *pBuffer, int nLen);

private:
	unsigned int m_unConnType;
};

#endif/*COMMUNOCATE_MESSAGE_SHOW_H_*/