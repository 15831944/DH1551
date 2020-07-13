
// UpgradeToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "StartRun/UpgradeSTB.h"
#include "DataManage/ConfigData/ParseConfig.h"
#include "UICommon.h"
#include "LoginInfo.h"

// CUpgradeToolDlg 对话框
class CUpgradeToolDlg : public CDialogEx
{
// 构造
public:
	CUpgradeToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPGRADETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);
	void SetToolVersion(void);

private:
	CEdit m_CEOrder;
	CComboBox m_ComWorkLine;
	CComboBox m_ComWorkID;
	CEdit m_CEPersonID;
	CButton m_RunBtn;
	CEdit m_ceVersion;
	CComboBox m_Coms;
	CEdit m_VerifySN;
	CEdit m_Result;
	ParseConfig m_ParseConfig;
	ConfigData m_ConfigData;
	CFont m_NormalFont;
	CButton m_ButtonEixt;
	UpgradeSTB m_UpgradeSTB;
	UICommon m_UICommon;
	LoginInfo m_LoginInfo;

public:
	afx_msg void OnBnClickedButtonEixt();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnCbnSelchangeCombo1();
	BOOL IsProgramRunning();
	unsigned int InitTool();
	void InitSerialCom(char * pacStr, unsigned int unLength);
	void ConfigPrinter();
	void EnableAllItem(BOOL bEnable);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEdit1();

	afx_msg void OnCheckDBData();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ShowResultMsg(void);
	afx_msg void ShowConfigInfo();
	int GetLoginInfo(void);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
