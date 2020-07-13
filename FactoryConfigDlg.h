#pragma once

#include "DataManage/ConfigData/ParseConfig.h"
#include "UICommon.h"
#include "afxwin.h"

#define CONFIG_FILE_NAME ("OrderConfig.txt")

// FactoryConfigDlg �Ի���

class FactoryConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FactoryConfigDlg)

public:
	FactoryConfigDlg(ConfigData & csConfigData, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FactoryConfigDlg();

// �Ի�������
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

private:
	ParseConfig m_ParseConfig;
	ConfigData & m_ConfigData;
	UICommon m_UICommon;

public:
	afx_msg void OnCbnSelchangeComboConnect();
	afx_msg void OnBnClickedCheckSn();
	afx_msg void OnBnClickedButtonTestPersonal();
	afx_msg void OnBnClickedCheckPersonal();
	void InitSerialComboBox(void);
	CButton m_CBBackDoor;
	BOOL m_CBConnectMES;
};
