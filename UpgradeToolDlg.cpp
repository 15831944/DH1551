
// UpgradeToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpgradeTool.h"
#include "UpgradeToolDlg.h"
#include "afxdialogex.h"
#include "ToolVersion/ToolVersion.h"
#include "FactoryConfigDlg.h"
#include "MesDBTypeInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_COM_COUNT 20
#define SHOW_MSG_TIMER (1234)

static HANDLE g_hMutex = NULL;
int g_BackDoorFlag = 0;
int g_DisconnectMesFlag = 0;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUpgradeToolDlg �Ի���



CUpgradeToolDlg::CUpgradeToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpgradeToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpgradeToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, m_ceVersion);
	DDX_Control(pDX, IDC_COMBO1, m_Coms);
	DDX_Control(pDX, IDC_EDIT1, m_VerifySN);
	DDX_Control(pDX, IDC_EDIT2, m_Result);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonEixt);
	DDX_Control(pDX, IDC_EDIT_ORDERID, m_CEOrder);
	DDX_Control(pDX, IDC_COMBO_LINE, m_ComWorkLine);
	DDX_Control(pDX, IDC_COMBO_STATION, m_ComWorkID);
	DDX_Control(pDX, IDC_EDIT_JOBNUM, m_CEPersonID);
	DDX_Control(pDX, IDC_BUTTON3, m_RunBtn);
}

BEGIN_MESSAGE_MAP(CUpgradeToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(MSG_USER_MSG, &CUpgradeToolDlg::OnUserMessage)
	ON_BN_CLICKED(IDC_BUTTON1, &CUpgradeToolDlg::OnBnClickedButtonEixt)
	ON_BN_CLICKED(IDC_BUTTON2, &CUpgradeToolDlg::OnBnClickedButtonPrint)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CUpgradeToolDlg::OnCbnSelchangeCombo1)
	ON_WM_CTLCOLOR()
	ON_WM_FONTCHANGE()
	ON_EN_CHANGE(IDC_EDIT1, &CUpgradeToolDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CUpgradeToolDlg::OnCheckDBData)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CUpgradeToolDlg::ShowConfigInfo)
	ON_BN_CLICKED(IDC_BUTTON4, &CUpgradeToolDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CUpgradeToolDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CUpgradeToolDlg ��Ϣ�������

BOOL CUpgradeToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	if (!IsProgramRunning() || InitTool() == 0)
	{
		//Exit Dialog
		OnCancel();
		return FALSE;
	}
	//ShellExecute(NULL, _T("open"), "F:\\MkUpGradeFile\\TestAppBackDoor\\UpgradeTool\\Debug\\UpgradeTool.exe", NULL, "", SW_SHOWNORMAL);
	m_UICommon.SetComboBoxSize(&m_ComWorkLine, 14);
	m_UICommon.SetComboBoxSize(&m_ComWorkID, 2);
	m_UICommon.CreatPrivateFont(&m_NormalFont, 30);

	m_Result.SetFont(&m_NormalFont);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUpgradeToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUpgradeToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUpgradeToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUpgradeToolDlg::SetToolVersion(void)
{
	char acToolVersion[32] = { 0 };
	char acVersion[32] = { 0 };

	if (GetToolVersion(acVersion, sizeof(acVersion)))
	{
		sprintf_s(acToolVersion, sizeof(acToolVersion), "%s-%s", m_ConfigData.m_acProject, acVersion);
		m_ceVersion.SetWindowText(acToolVersion);
		m_ceVersion.EnableWindow(FALSE);
	}
}

void CUpgradeToolDlg::OnBnClickedButtonEixt()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	KillTimer(SHOW_MSG_TIMER);

	m_UpgradeSTB.CloseConn();

	::CloseHandle(g_hMutex);

	CDialogEx::OnOK();
}

afx_msg LRESULT CUpgradeToolDlg::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case MSG_USER_MSG_SET_VERIFY_FOCUS:
		m_VerifySN.SetFocus();
		break;

	default:
		break;
	}

	return 0;
}

void CUpgradeToolDlg::OnBnClickedButtonPrint()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	m_UpgradeSTB.RebuiltPrintFile();//���´�ӡ
}

void CUpgradeToolDlg::OnCbnSelchangeCombo1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//char acComName[8] = { 0 };

	CString string;

	m_Coms.GetLBText(m_Coms.GetCurSel(), string);

	if (memcmp(string.GetBuffer(0), m_ConfigData.m_acSerial, string.GetLength()) != 0)
	{
		m_UpgradeSTB.Reconnect(string.GetBuffer(0), string.GetLength());
		memset(m_ConfigData.m_acSerial, 0, sizeof(m_ConfigData.m_acSerial));
		memcpy(m_ConfigData.m_acSerial, string.GetBuffer(0), string.GetLength());
		if (m_ParseConfig.SaveComChange(string.GetBuffer(0), CONFIG_FILE_NAME, strlen(CONFIG_FILE_NAME)) == 0)
		{
			MessageBox(_T("Can't find config file or File error!"), 0, MB_OK);
		}
	}

	::PostMessageW(GetSafeHwnd(), MSG_USER_MSG, MSG_USER_MSG_SET_VERIFY_FOCUS, 0);

}

BOOL CUpgradeToolDlg::IsProgramRunning()
{
	g_hMutex = ::CreateMutex(NULL, FALSE, _T("RunningMutex"));//����������
	DWORD dwRet = ::GetLastError();

	if (g_hMutex)
	{
		if (ERROR_ALREADY_EXISTS == dwRet)
		{
			AfxMessageBox(_T("�����Ѿ�������"));
			CloseHandle(g_hMutex);  // should be closed
			return FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("Create Mutex Error"));
		return FALSE;
	}

	return TRUE;
}

unsigned int CUpgradeToolDlg::InitTool()
{
	unsigned int unRet = 0;

	if (m_ParseConfig.ReadConfigFile(CONFIG_FILE_NAME, strlen(CONFIG_FILE_NAME)) > 0)
	{
		m_ParseConfig.GetConfigData(m_ConfigData);
		ConfigPrinter();
		SetToolVersion();

		if (m_ConfigData.m_nConnType == 0)
		{
			InitSerialCom(m_ConfigData.m_acSerial, sizeof(m_ConfigData.m_acSerial));
		}
		else
		{
			m_Coms.EnableWindow(FALSE);
		}

		if (m_LoginInfo.ReadLoginData(m_ConfigData.m_acOrderNum))
		{
			m_ComWorkLine.SetCurSel(m_ComWorkLine.FindStringExact(0, m_LoginInfo.m_acLine));
			m_ComWorkID.SetCurSel(m_ComWorkID.FindStringExact(0, m_LoginInfo.m_acStep));
			m_CEPersonID.SetWindowText(m_LoginInfo.m_acPersonID);
		}
		m_CEOrder.SetWindowText(m_ConfigData.m_acOrderNum);
		m_VerifySN.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
		unRet = 1;
	}
	else
	{
		MessageBox(_T("Can't find config file or File error!"), 0, MB_OK);
	}

	return unRet;
}

void CUpgradeToolDlg::InitSerialCom(char * pacStr, unsigned int unLength)
{
	unsigned int ii;
	int nIndex = 0;
	char acStr[8] = { 0 };

	for (ii = 0; ii <= MAX_COM_COUNT; ii++)
	{
		sprintf_s(acStr, sizeof(acStr), "COM%d", ii + 1);
		m_Coms.InsertString(ii, acStr);
		memset(acStr, 0, sizeof(acStr));
	}
	m_UICommon.SetComboBoxSize(&m_Coms, 20);

	if (pacStr != NULL)
	{
		nIndex = m_Coms.FindStringExact(0, pacStr);
		m_Coms.SetCurSel(nIndex);
	}
}

void CUpgradeToolDlg::ConfigPrinter()
{
	FILE *pFILE = NULL;
	char acFilePath[256] = { 0 };
	char acBuffer[2 * 1024] = { 0 };

	GetCurrentDirectoryA(sizeof(acFilePath), acFilePath);
	sprintf_s(acBuffer, sizeof(acBuffer), "copy /Y %s\\id_print.txt D:\\BarCode\\id_print.bak\r\n", acFilePath);
	sprintf_s(acBuffer + strlen(acBuffer), sizeof(acBuffer)-strlen(acBuffer), "move /Y %s\\id_print.txt D:\\BarCode\\id_print.dat\r\n", acFilePath);
	sprintf_s(acBuffer + strlen(acBuffer), sizeof(acBuffer)-strlen(acBuffer), "C:\\PROGRA~1\\Seagull\\BARTEN~1\\7.75\\bartend.exe /AF=D:\\BarCode\\%s-���Ӱٲʺ�.btw /P /C=%d /D=D:\\BarCode\\id_print.dat /DD \r\n", m_ConfigData.m_acProject, m_ConfigData.m_BarCode);

	fopen_s(&pFILE, "D:\\BarCode\\barprint.bat", "wb+");
	if (pFILE != NULL)
	{
		fwrite(acBuffer, strlen(acBuffer), sizeof(char), pFILE);
		fclose(pFILE);
	}
}

void CUpgradeToolDlg::EnableAllItem(BOOL bEnable)
{
	m_Coms.EnableWindow(bEnable);
	m_ButtonEixt.EnableWindow(bEnable);
	m_VerifySN.EnableWindow(bEnable);
	m_Result.EnableWindow(bEnable);
}

HBRUSH CUpgradeToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

	if (pWnd->GetDlgCtrlID() == IDC_EDIT2)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}

	return hbr;
}

void CUpgradeToolDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString string;
	CString strShow;
	m_Result.SetFont(&m_NormalFont);

	if (m_VerifySN.GetWindowTextLength() == m_ConfigData.m_nSNLen)
	{
		m_VerifySN.GetWindowText(string);
		if (g_DisconnectMesFlag)
		{
			m_UpgradeSTB.SetBarCode(string.GetBuffer(0), string.GetLength());
			m_VerifySN.SetWindowText(_T(""));
			strShow.Format("SN:%s\n", string.GetBuffer());
			m_Result.SetWindowText(strShow);
		}
		else
		{
			if (m_UpgradeSTB.CheckWorkPoint(string.GetBuffer(0), string.GetLength()) == 0)
			{
				m_Result.SetWindowText(_T("��ǰMES�ڵ����"));
			}
			else
			{
				m_UpgradeSTB.SetBarCode(string.GetBuffer(0), string.GetLength());
				m_VerifySN.SetWindowText(_T(""));
				strShow.Format("SN:%s\n", string.GetBuffer());
				m_Result.SetWindowText(strShow);
			}
		}
		
		m_VerifySN.SetWindowText(_T(""));
	}
	else if (m_VerifySN.GetWindowTextLength() > m_ConfigData.m_nSNLen)
	{
		m_Result.SetWindowText(_T("\r\n���к���������,������ɨ��"));
		UpdateData(TRUE);
	}
}


void CUpgradeToolDlg::OnCheckDBData()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (GetLoginInfo() > 0)
	{
		if (m_UpgradeSTB.CheckOrderInfo(m_LoginInfo.m_acOrderID, m_LoginInfo.m_acStep, m_LoginInfo.m_acLine, m_LoginInfo.m_acPersonID))
		{
			m_UpgradeSTB.SetConfigData(m_ConfigData);

			if (((CButton*)GetDlgItem(IDC_CHECK_MES))->GetCheck())
			{
				MesDBTypeInfoDlg tempMesDBTypeInfoDlg;
				char acStr[128] = { 0 };
				unsigned int unDBType = m_UpgradeSTB.GetDBDataType();

				m_UpgradeSTB.GetDBSTBType(acStr, sizeof(acStr));
				tempMesDBTypeInfoDlg.SetDBTypeAndTypeName(unDBType, acStr, strlen(acStr));
				tempMesDBTypeInfoDlg.DoModal();
			}

			if (m_ConfigData.m_nConnType == 0)
			{
				m_UpgradeSTB.Init(m_ConfigData.m_acSerial, strlen(m_ConfigData.m_acSerial), m_ConfigData.m_nConnType);
			}
			else
			{
				m_UpgradeSTB.Init(m_ConfigData.m_acLocalIP, m_ConfigData.m_LocalPort, m_ConfigData.m_nConnType);
			}

			SetTimer(SHOW_MSG_TIMER, 500, NULL);

			::PostMessageW(GetSafeHwnd(), MSG_USER_MSG, MSG_USER_MSG_SET_VERIFY_FOCUS, 0);

			m_CEOrder.EnableWindow(FALSE);
			m_ComWorkLine.EnableWindow(FALSE);
			m_ComWorkID.EnableWindow(FALSE);
			m_CEPersonID.EnableWindow(FALSE);
			m_RunBtn.EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_MES)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(FALSE);
			if (memcmp(m_LoginInfo.m_acStep, "У��", sizeof("У��")) == 0)
			{
				m_VerifySN.EnableWindow(TRUE);
			}
		}
		else
		{
			MessageBox(_T("������Ϣ�����Ա���Ų�����!"));
		}
	}
}

int CUpgradeToolDlg::GetLoginInfo(void)
{
	int nRet = 0;
	CString strOder;
	CString strLine;
	CString strTestDevice;
	CString strJobNumber;

	do
	{
		if (m_CEOrder.GetWindowTextLengthA() == 0)
		{
			AfxMessageBox(_T("�����Ų���Ϊ��!"));
			break;
		}

		if (m_ComWorkLine.GetWindowTextLengthA() == 0)
		{
			AfxMessageBox(_T("���ߺŲ���Ϊ��!"));
			break;
		}

		if (m_ComWorkID.GetWindowTextLengthA() == 0)
		{
			AfxMessageBox(_T("��λ�Ų���Ϊ��!"));
			break;
		}

		if (m_CEPersonID.GetWindowTextLengthA() == 0)
		{
			AfxMessageBox(_T("����Ա���Ų���Ϊ��!"));
			break;
		}
		GetDlgItem(IDC_EDIT_ORDERID)->GetWindowTextA(strOder);
		GetDlgItem(IDC_COMBO_LINE)->GetWindowTextA(strLine);
		GetDlgItem(IDC_COMBO_STATION)->GetWindowTextA(strTestDevice);
		GetDlgItem(IDC_EDIT_JOBNUM)->GetWindowTextA(strJobNumber);

		m_LoginInfo.SaveLoginData(strOder.GetBuffer(0), strLine.GetBuffer(0), strTestDevice.GetBuffer(0), strJobNumber.GetBuffer(0));

		nRet = 1;

	} while (0);

	return nRet;
}

void CUpgradeToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch (nIDEvent)
	{
	case SHOW_MSG_TIMER:
		ShowResultMsg();
		break;

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CUpgradeToolDlg::ShowResultMsg(void)
{
	char acLog[800] = { 0 };
	static char acLast[800] = { 0 };

	memset(acLog, 0, sizeof(acLog));

	if (m_UpgradeSTB.ReadLog(acLog, sizeof(acLog) - 3))
	{
		if (strlen(acLog) > 0 && memcmp(acLast, acLog, strlen(acLog)) != 0)
		{
			memcpy(acLast, acLog, strlen(acLog));

			memcpy(&acLog[strlen(acLog)], "\r\n", 3);

			unsigned int iLen = 0;

			iLen = m_Result.GetWindowTextLength();

			if (iLen + strlen(acLog) >= m_Result.GetLimitText())
			{
				m_Result.SetWindowText("");

			}
			m_Result.SetSel(iLen, iLen, TRUE);
			m_Result.ReplaceSel(acLog);
		}
	}
}

void CUpgradeToolDlg::ShowConfigInfo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	FactoryConfigDlg tempFactoryConfigDlg(m_ConfigData);

	int nEnd = tempFactoryConfigDlg.DoModal();

	if (nEnd == IDC_BUTTON_SAVE)
	{
		ConfigPrinter();

		SetToolVersion();

		if (m_ConfigData.m_nConnType == 0)
		{
			InitSerialCom(m_ConfigData.m_acSerial, sizeof(m_ConfigData.m_acSerial));
		}
		m_Coms.EnableWindow(!m_ConfigData.m_nConnType);

		m_CEOrder.SetWindowTextA(m_ConfigData.m_acOrderNum);

		if (g_BackDoorFlag)
		{
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		}
	}
}


void CUpgradeToolDlg::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_UpgradeSTB.StartRuqestReturnFile();
}


void CUpgradeToolDlg::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_UpgradeSTB.StartEraseAllData();
}
