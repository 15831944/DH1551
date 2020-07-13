// FactoryConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpgradeTool.h"
#include "FactoryConfigDlg.h"
#include "afxdialogex.h"
#include "ConaxPersonalizeDlg.h"

extern int g_BackDoorFlag;
extern int g_DisconnectMesFlag;
// FactoryConfigDlg 对话框

IMPLEMENT_DYNAMIC(FactoryConfigDlg, CDialogEx)

FactoryConfigDlg::FactoryConfigDlg(ConfigData & csConfigData, CWnd* pParent /*=NULL*/)
: CDialogEx(FactoryConfigDlg::IDD, pParent), m_ConfigData(csConfigData)
, m_CBConnectMES(FALSE)
{

}

FactoryConfigDlg::~FactoryConfigDlg()
{
}

void FactoryConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CBBackDoor);
	DDX_Check(pDX, IDC_CHECK2, m_CBConnectMES);
}


BEGIN_MESSAGE_MAP(FactoryConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &FactoryConfigDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_CONNECT, &FactoryConfigDlg::OnCbnSelchangeComboConnect)
	ON_BN_CLICKED(IDC_CHECK_SN, &FactoryConfigDlg::OnBnClickedCheckSn)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PERSONAL, &FactoryConfigDlg::OnBnClickedButtonTestPersonal)
	ON_BN_CLICKED(IDC_CHECK_PERSONAL, &FactoryConfigDlg::OnBnClickedCheckPersonal)
END_MESSAGE_MAP()


// FactoryConfigDlg 消息处理程序

BOOL FactoryConfigDlg::OnInitDialog()
{
	m_UICommon.SetComboBoxSize((CComboBox*)GetDlgItem(IDC_COMBO_CONNECT), 3);
	m_UICommon.SetComboBoxSize((CComboBox*)GetDlgItem(IDC_COMBO_SERIAL), 10);
	m_UICommon.SetComboBoxSize((CComboBox*)GetDlgItem(IDC_COMBO_MODEL), 10);
	
	InitSerialComboBox();

	((CComboBox*)GetDlgItem(IDC_COMBO_CONNECT))->AddString("串口");
	((CComboBox*)GetDlgItem(IDC_COMBO_CONNECT))->AddString("网口");

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->AddString("烧号");
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->AddString("维修");
	//((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->AddString("打印");
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->EnableWindow(FALSE);

	//if (m_ParseConfig.ReadConfigFile(CONFIG_FILE_NAME, strlen(CONFIG_FILE_NAME)) > 0)
	{
		CString str;
		//m_ParseConfig.GetConfigData(m_ConfigData);

		((CComboBox*)GetDlgItem(IDC_COMBO_CONNECT))->SetCurSel(m_ConfigData.m_nConnType);

		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetCurSel(0);//(m_ConfigData.m_nToolMode);

		GetDlgItem(IDC_COMBO_SERIAL)->SetWindowTextA(m_ConfigData.m_acSerial);
		str.Format("%u", m_ConfigData.m_LocalPort);
		GetDlgItem(IDC_IPADDRESS_STB)->SetWindowTextA(m_ConfigData.m_acLocalIP);
		GetDlgItem(IDC_EDIT_STB)->SetWindowTextA(str);
		if (m_ConfigData.m_nConnType == 0)
		{
			GetDlgItem(IDC_IPADDRESS_STB)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_STB)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_COMBO_SERIAL)->EnableWindow(FALSE);
		}

		//server info
		str.Format("%u", m_ConfigData.m_ServerPort);
		GetDlgItem(IDC_IPADDRESS_SERVER)->SetWindowTextA(m_ConfigData.m_acServerIP);
		GetDlgItem(IDC_EDIT_SERVER)->SetWindowTextA(str);

		//project info
		GetDlgItem(IDC_EDIT_PROJECT)->SetWindowTextA(m_ConfigData.m_acProject);
		((CButton*)GetDlgItem(IDC_CHECK_SN))->SetCheck(m_ConfigData.m_nSN);
		str.Format("%d", m_ConfigData.m_nSNLen);
		GetDlgItem(IDC_EDIT_SNLEN)->SetWindowTextA(str);
		if (m_ConfigData.m_nSN != 1)
		{
			GetDlgItem(IDC_EDIT_SNLEN)->EnableWindow(FALSE);
		}
		((CButton*)GetDlgItem(IDC_CHECK_HDCP))->SetCheck(m_ConfigData.m_nHDCP);
		((CButton*)GetDlgItem(IDC_CHECK_MAC))->SetCheck(m_ConfigData.m_nMAC);
		((CButton*)GetDlgItem(IDC_CHECK_PEF))->SetCheck(m_ConfigData.m_nPEF);
		((CButton*)GetDlgItem(IDC_CHECK_PERSONAL))->SetCheck(m_ConfigData.m_nPersonal);
		if (m_ConfigData.m_nPersonal != 1)
		{
			GetDlgItem(IDC_IPADDRESS_PERSONAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PERSONAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_TEST_PERSONAL)->EnableWindow(FALSE);
		}
		str.Format("%u", m_ConfigData.m_PersonalPort);
		GetDlgItem(IDC_IPADDRESS_PERSONAL)->SetWindowTextA(m_ConfigData.m_acPersonalIP);
		GetDlgItem(IDC_EDIT_PERSONAL)->SetWindowTextA(str);
		GetDlgItem(IDC_EDIT_PLATID)->SetWindowTextA(m_ConfigData.m_acPlatID);

		//order info
		str.Format("%d", m_ConfigData.m_BarCode);
		GetDlgItem(IDC_EDIT_ORDER)->SetWindowTextA(m_ConfigData.m_acOrderNum);
		GetDlgItem(IDC_EDIT_CODE)->SetWindowTextA(str);
	}

	return TRUE;
}

void FactoryConfigDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;

	memset(&m_ConfigData, 0, sizeof(ConfigData));

	m_ConfigData.m_nConnType = ((CComboBox*)GetDlgItem(IDC_COMBO_CONNECT))->GetCurSel();

	m_ConfigData.m_nToolMode = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCurSel();
	
	GetDlgItem(IDC_COMBO_SERIAL)->GetWindowTextA(str);
	memcpy(m_ConfigData.m_acSerial, str.GetBuffer(0), str.GetLength());

	GetDlgItem(IDC_IPADDRESS_STB)->GetWindowTextA(str);
	memcpy(m_ConfigData.m_acLocalIP, str.GetBuffer(0), str.GetLength());
	GetDlgItem(IDC_EDIT_STB)->GetWindowTextA(str);
	m_ConfigData.m_LocalPort = atoi(str.GetBuffer(0));

	//server info
	GetDlgItem(IDC_IPADDRESS_SERVER)->GetWindowTextA(str);
	memcpy(m_ConfigData.m_acServerIP, str.GetBuffer(0), str.GetLength());
	GetDlgItem(IDC_EDIT_SERVER)->GetWindowTextA(str);
	m_ConfigData.m_ServerPort = atoi(str.GetBuffer(0));

	//project info
	GetDlgItem(IDC_EDIT_PROJECT)->GetWindowTextA(str);
	memcpy(m_ConfigData.m_acProject, str.GetBuffer(0), str.GetLength());
	m_ConfigData.m_nSN = ((CButton*)GetDlgItem(IDC_CHECK_SN))->GetCheck();
	
	GetDlgItem(IDC_EDIT_SNLEN)->GetWindowTextA(str);
	m_ConfigData.m_nSNLen = atoi(str.GetBuffer(0));
	
	m_ConfigData.m_nHDCP = ((CButton*)GetDlgItem(IDC_CHECK_HDCP))->GetCheck();
	m_ConfigData.m_nMAC = ((CButton*)GetDlgItem(IDC_CHECK_MAC))->GetCheck();
	m_ConfigData.m_nPEF = ((CButton*)GetDlgItem(IDC_CHECK_PEF))->GetCheck();
	m_ConfigData.m_nPersonal = ((CButton*)GetDlgItem(IDC_CHECK_PERSONAL))->GetCheck();
	
	GetDlgItem(IDC_IPADDRESS_PERSONAL)->GetWindowTextA(str);
	memcpy(m_ConfigData.m_acPersonalIP, str.GetBuffer(0), str.GetLength());

	GetDlgItem(IDC_EDIT_PERSONAL)->GetWindowTextA(str);
	m_ConfigData.m_PersonalPort = atoi(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT_PLATID)->GetWindowTextA(str);
	memcpy(m_ConfigData.m_acPlatID, str.GetBuffer(0), str.GetLength());


	//order info
	GetDlgItem(IDC_EDIT_ORDER)->GetWindowTextA(str);
	memcpy(m_ConfigData.m_acOrderNum, str.GetBuffer(0), str.GetLength());
	GetDlgItem(IDC_EDIT_CODE)->GetWindowTextA(str);
	m_ConfigData.m_BarCode = atoi(str.GetBuffer(0));

	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		g_BackDoorFlag = 1;
	}

	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		g_DisconnectMesFlag = 1;
	}

	if (m_ParseConfig.WriteConfigFile(CONFIG_FILE_NAME, strlen(CONFIG_FILE_NAME), m_ConfigData))
	{
		AfxMessageBox(_T("保存成功！"));

		EndDialog(IDC_BUTTON_SAVE);
	}
	else
	{
		AfxMessageBox(_T("保存失败！"));
	}
}

void FactoryConfigDlg::OnCbnSelchangeComboConnect()
{
	// TODO:  在此添加控件通知处理程序代码

	m_ConfigData.m_nConnType = ((CComboBox*)GetDlgItem(IDC_COMBO_CONNECT))->GetCurSel();

	BOOL bEnable = (m_ConfigData.m_nConnType == 0) ? FALSE : TRUE;

	GetDlgItem(IDC_IPADDRESS_STB)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_STB)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_SERIAL)->EnableWindow(!bEnable);
}


void FactoryConfigDlg::OnBnClickedCheckSn()
{
	// TODO:  在此添加控件通知处理程序代码

	m_ConfigData.m_nSN = ((CButton*)GetDlgItem(IDC_CHECK_SN))->GetCheck();

	BOOL bEnable = (m_ConfigData.m_nSN == 0) ? FALSE : TRUE;

	GetDlgItem(IDC_EDIT_SNLEN)->EnableWindow(bEnable);
}


void FactoryConfigDlg::OnBnClickedButtonTestPersonal()
{
	// TODO:  在此添加控件通知处理程序代码

	if (m_ConfigData.m_nPersonal)
	{
		ConaxPersonalizeDlg dlg;

		dlg.SetNetwork(m_ConfigData.m_acPersonalIP, m_ConfigData.m_PersonalPort);

		dlg.DoModal();
	}
}


void FactoryConfigDlg::OnBnClickedCheckPersonal()
{
	// TODO:  在此添加控件通知处理程序代码

	m_ConfigData.m_nPersonal = ((CButton*)GetDlgItem(IDC_CHECK_PERSONAL))->GetCheck();

	BOOL bEnable = (m_ConfigData.m_nPersonal == 0) ? FALSE : TRUE;

	GetDlgItem(IDC_IPADDRESS_PERSONAL)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_PERSONAL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_TEST_PERSONAL)->EnableWindow(bEnable);

}

void FactoryConfigDlg::InitSerialComboBox(void)
{
	COMMCONFIG cfg;
	char port[10] = { 0 };
	char openport[20] = { 0 };
	DWORD  t;
	HANDLE h;
	int k = 0;

	for (k = 1; k <= 64; k++)
	{
		sprintf(port, "COM%d", k);

		if (k >= 10)
		{
			sprintf(openport, "\\\\.\\%s", port);
		}
		else
		{
			sprintf(openport, "%s", port);
		}

		h = CreateFileA(openport, GENERIC_READ | GENERIC_WRITE, 0,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE != h)
		{
			t = sizeof(cfg);
			if (GetCommConfig(h, &cfg, &t))
			{
				((CComboBox*)GetDlgItem(IDC_COMBO_SERIAL))->AddString(port);
			}
			CloseHandle(h);
		}
	}
}
