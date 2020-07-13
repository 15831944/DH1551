// ConaxPersonalizeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpgradeTool.h"
#include "ConaxPersonalizeDlg.h"
#include "afxdialogex.h"


// ConaxPersonalizeDlg 对话框

IMPLEMENT_DYNAMIC(ConaxPersonalizeDlg, CDialogEx)

ConaxPersonalizeDlg::ConaxPersonalizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ConaxPersonalizeDlg::IDD, pParent)
{
	memset(m_acIP, 0, sizeof(m_acIP));

	m_usPort = 0;
}

ConaxPersonalizeDlg::~ConaxPersonalizeDlg()
{
}

void ConaxPersonalizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ConaxPersonalizeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CONNECT, &ConaxPersonalizeDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_INIT_JOB, &ConaxPersonalizeDlg::OnBnClickedInitJob)
	ON_BN_CLICKED(IDC_ENCRYTIONKEY, &ConaxPersonalizeDlg::OnBnClickedEncrytionkey)
	ON_BN_CLICKED(IDC_DELETE_JOB, &ConaxPersonalizeDlg::OnBnClickedDeleteJob)
	ON_BN_CLICKED(IDC_VSC_DATA, &ConaxPersonalizeDlg::OnBnClickedVscData)
	ON_BN_CLICKED(IDC_BUTTON1, &ConaxPersonalizeDlg::OnLoadProductReport)
END_MESSAGE_MAP()


// ConaxPersonalizeDlg 消息处理程序
BOOL ConaxPersonalizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strIP;
	CString strPort;

	if (strlen(m_acIP) != 0 && m_usPort != 0)
	{
		strIP.Format("%s", m_acIP);

		strPort.Format("%u", m_usPort);

		SetDlgItemText(IDC_IPADDRESS_SERVER, strIP);

		SetDlgItemText(IDC_EDIT_PORT, strPort);
	}

	return TRUE;
}

void ConaxPersonalizeDlg::OnBnClickedConnect()
{
	// TODO:  在此添加控件通知处理程序代码

	CString strIP;
	CString strPort;
	unsigned short usPort;

	GetDlgItemText(IDC_IPADDRESS_SERVER, strIP);

	GetDlgItemText(IDC_EDIT_PORT, strPort);

	if (strIP.GetLength() == 0 || strPort.GetLength() == 0)
	{
		AfxMessageBox(_T("IP或PORT不能为空"));
	}
	else
	{
		usPort = atoi(strPort.GetBuffer(0));

		m_ConaxPersonalize.SetNetworkInfo(strIP.GetBuffer(0), strIP.GetLength(), usPort);

		if (m_ConaxPersonalize.GetHomePage() == 0)
		{
			AfxMessageBox(_T("连接成功"));

			GetDlgItem(IDC_CONNECT)->EnableWindow(0);
		}
		else
		{
			AfxMessageBox(_T("连接失败"));
		}

		char *temp = NULL;
		unsigned int unLen = 0;
		GetLogBuffer(&temp, &unLen);
		if (unLen > 0 && temp != NULL)
		{
			SetDlgItemText(IDC_RESULT, temp);
			ClearLogBuffer();
		}
	}
}


void ConaxPersonalizeDlg::OnBnClickedInitJob()
{
	// TODO:  在此添加控件通知处理程序代码

	if (!(GetDlgItem(IDC_CONNECT)->GetStyle() & WS_DISABLED))
	{
		AfxMessageBox(_T("请先连接个性化服务器！"));
		return;
	}

	CString strJobFile;

	GetDlgItemText(IDC_KEYFILE, strJobFile);

	if (strJobFile.GetLength() == 0)
	{
		AfxMessageBox(_T("KeyFile不能为空"));
	}
	else
	{
		if (m_ConaxPersonalize.InitJobFile(strJobFile.GetBuffer(), strJobFile.GetLength()) == 1)
		{
			AfxMessageBox(_T("KeyFile加载成功"));
		}

		char *temp = NULL;
		unsigned int unLen = 0;
		GetLogBuffer(&temp, &unLen);
		if (unLen > 0 && temp != NULL)
		{
			SetDlgItemText(IDC_RESULT, temp);
			ClearLogBuffer();
		}
	}
}


void ConaxPersonalizeDlg::OnBnClickedEncrytionkey()
{
	// TODO:  在此添加控件通知处理程序代码

	return;

	CString strChipID;

	GetDlgItemText(IDC_CHIPID, strChipID);

	if (strChipID.GetLength() == 0)
	{
		AfxMessageBox(_T("jobid或chipid不能为空"));
	}
	else
	{
		unsigned int chipid = atoi(strChipID.GetBuffer(0));
		unsigned char acEncryptKey[256] = { 0 };
		unsigned int unKeyLen = sizeof(acEncryptKey);
		if (m_ConaxPersonalize.RequestEncrytionKey(chipid, acEncryptKey, &unKeyLen) == 1)
		{
			AfxMessageBox(_T("获取加密KEY成功"));
		}

		char *temp = NULL;
		unsigned int unLen = 0;
		GetLogBuffer(&temp, &unLen);
		if (unLen > 0 && temp != NULL)
		{
			SetDlgItemText(IDC_RESULT, temp);
			ClearLogBuffer();
		}
	}
}


void ConaxPersonalizeDlg::OnBnClickedDeleteJob()
{
	// TODO:  在此添加控件通知处理程序代码

	AfxMessageBox(_T("禁止删除KeyFile！"));
	return;
	if (AfxMessageBox(_T("禁止删除KeyFile,是否强制删除？"), MB_YESNO) == IDNO)
	{
		return;
	}

	if (m_ConaxPersonalize.DeleteJobFile())
	{
		AfxMessageBox(_T("删除成功"));
	}

	char *temp = NULL;
	unsigned int unLen = 0;
	GetLogBuffer(&temp, &unLen);
	if (unLen > 0 && temp != NULL)
	{
		SetDlgItemText(IDC_RESULT, temp);
		ClearLogBuffer();
	}
}


void ConaxPersonalizeDlg::OnBnClickedVscData()
{
	// TODO:  在此添加控件通知处理程序代码

	return ;

	CString strChipID;

	GetDlgItemText(IDC_CHIPID, strChipID);

	if (strChipID.GetLength() == 0)
	{
		AfxMessageBox(_T("jobid或chipid不能为空"));
	}
	else
	{
		unsigned int chipid = atoi(strChipID.GetBuffer(0));
		unsigned char vscData[64*1024] = { 0 };
		unsigned int unDataLen = sizeof(vscData);

		if (m_ConaxPersonalize.RequestVSCData(vscData, &unDataLen))
		{
			AfxMessageBox(_T("获取VSC DATA 成功"));
		}

		char *temp = NULL;
		unsigned int unLen = 0;
		GetLogBuffer(&temp, &unLen);
		if (unLen > 0 && temp != NULL)
		{
			SetDlgItemText(IDC_RESULT, temp);
			ClearLogBuffer();
		}
	}
}

void ConaxPersonalizeDlg::SetNetwork(char *pIP, unsigned short usPort)
{
	if (pIP != NULL)
	{
		memcpy(m_acIP, pIP, strlen(pIP));

		m_usPort = usPort;
	}
}

void ConaxPersonalizeDlg::OnLoadProductReport()
{
	// TODO:  在此添加控件通知处理程序代码

	CString str;
	FILE *fp = NULL;

	GetDlgItem(IDC_MFCEDITBROWSE1)->GetWindowTextA(str);

	if (str.GetLength() == 0)
	{
		AfxMessageBox(_T("请选择当前returnfile，returnfile不能为空！"));
		return;
	}

	fopen_s(&fp, str.GetBuffer(0), "rb");
	if (fp == NULL)
	{
		AfxMessageBox(_T("无法打开所选文件！"));
		return;
	}
	else
	{
		fclose(fp);

		fp = NULL;
	}

	m_ConaxPersonalize.SetInputCSVFilePath(str.GetBuffer(0));

	CString strIP;
	CString strPort;
	unsigned short usPort;

	GetDlgItemText(IDC_IPADDRESS_SERVER, strIP);

	GetDlgItemText(IDC_EDIT_PORT, strPort);

	if (strIP.GetLength() == 0 || strPort.GetLength() == 0)
	{
		AfxMessageBox(_T("IP或PORT不能为空"));
		return;
	}
	
	usPort = atoi(strPort.GetBuffer(0));

	m_ConaxPersonalize.SetNetworkInfo(strIP.GetBuffer(0), strIP.GetLength(), usPort);


	CString	returnFile;
	static unsigned char gVSCReport[0x500000] = {0}; // Max 5M
	unsigned int unLen = 0;
	char returnFilename[128] = {0};
	if (m_ConaxPersonalize.PrepareReportFile(gVSCReport, &unLen, returnFilename) == 0)
	{
		AfxMessageBox(_T("生成生产报告成功, 请选择保存路径！"));

		CFileDialog fileDlg(FALSE, "cfv", returnFilename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CXF Files (*.cxf)|*.cxf|All Files (*.*)|*.*||"), NULL);
		fileDlg.m_ofn.lpstrTitle = _T("Saving Return File(.cxf)");
		if (fileDlg.DoModal() != IDOK)
		{
			return;
		}

		returnFile = fileDlg.GetPathName();
		memcpy(returnFilename, returnFile.GetBuffer(0), returnFile.GetLength());
		returnFile.ReleaseBuffer();
		PrintfLog("returnFilename:[%s]\r\n", returnFilename);

		if ((fp = fopen(returnFilename, "wb")) == NULL)
		{
			AfxMessageBox("ERROR: Can't open m_pCSVFilepath file");
		}
		else
		{
			int reallyWriteLen = 0;

			reallyWriteLen = fwrite(gVSCReport, 1, unLen, fp); /* 写的struct文件*/

			PrintfLog("reallyWriteLen:%d unLen:%d\r\n", reallyWriteLen, unLen);

			fclose(fp); /*关闭文件*/
		}

	}
	else
	{
		AfxMessageBox(_T("生成生产报告失败！"));
	}
}
