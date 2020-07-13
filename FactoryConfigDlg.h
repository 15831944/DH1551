#pragma once

#include "DataManage/ConfigData/ParseConfig.h"
#include "UICommon.h"
#include "afxwin.h"

#define CONFIG_FILE_NAME ("OrderConfig.txt")

// FactoryConfigDlg 对话框

class FactoryConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FactoryConfigDlg)

public:
	FactoryConfigDlg(ConfigData & csConfigData, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FactoryConfigDlg();

// 对话框数据
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
