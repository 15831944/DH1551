#pragma once
#include "DataManage/ConaxPersonalize/ConaxPersonalize.h"
#include "DataManage/ConaxPersonalize/HttpClient/DebugLog.h"

// ConaxPersonalizeDlg �Ի���

class ConaxPersonalizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConaxPersonalizeDlg)

public:
	ConaxPersonalizeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ConaxPersonalizeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PERSONALIZE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedInitJob();
	afx_msg void OnBnClickedEncrytionkey();
	afx_msg void OnBnClickedDeleteJob();
	afx_msg void OnBnClickedVscData();
	void SetNetwork(char *pIP, unsigned short usPort);

private:
	char m_acIP[32];
	unsigned short m_usPort;
	ConaxPersonalize m_ConaxPersonalize;
public:
	afx_msg void OnLoadProductReport();
};
