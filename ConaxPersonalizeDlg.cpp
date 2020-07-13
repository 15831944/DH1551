// ConaxPersonalizeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpgradeTool.h"
#include "ConaxPersonalizeDlg.h"
#include "afxdialogex.h"


// ConaxPersonalizeDlg �Ի���

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


// ConaxPersonalizeDlg ��Ϣ�������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString strIP;
	CString strPort;
	unsigned short usPort;

	GetDlgItemText(IDC_IPADDRESS_SERVER, strIP);

	GetDlgItemText(IDC_EDIT_PORT, strPort);

	if (strIP.GetLength() == 0 || strPort.GetLength() == 0)
	{
		AfxMessageBox(_T("IP��PORT����Ϊ��"));
	}
	else
	{
		usPort = atoi(strPort.GetBuffer(0));

		m_ConaxPersonalize.SetNetworkInfo(strIP.GetBuffer(0), strIP.GetLength(), usPort);

		if (m_ConaxPersonalize.GetHomePage() == 0)
		{
			AfxMessageBox(_T("���ӳɹ�"));

			GetDlgItem(IDC_CONNECT)->EnableWindow(0);
		}
		else
		{
			AfxMessageBox(_T("����ʧ��"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (!(GetDlgItem(IDC_CONNECT)->GetStyle() & WS_DISABLED))
	{
		AfxMessageBox(_T("�������Ӹ��Ի���������"));
		return;
	}

	CString strJobFile;

	GetDlgItemText(IDC_KEYFILE, strJobFile);

	if (strJobFile.GetLength() == 0)
	{
		AfxMessageBox(_T("KeyFile����Ϊ��"));
	}
	else
	{
		if (m_ConaxPersonalize.InitJobFile(strJobFile.GetBuffer(), strJobFile.GetLength()) == 1)
		{
			AfxMessageBox(_T("KeyFile���سɹ�"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	return;

	CString strChipID;

	GetDlgItemText(IDC_CHIPID, strChipID);

	if (strChipID.GetLength() == 0)
	{
		AfxMessageBox(_T("jobid��chipid����Ϊ��"));
	}
	else
	{
		unsigned int chipid = atoi(strChipID.GetBuffer(0));
		unsigned char acEncryptKey[256] = { 0 };
		unsigned int unKeyLen = sizeof(acEncryptKey);
		if (m_ConaxPersonalize.RequestEncrytionKey(chipid, acEncryptKey, &unKeyLen) == 1)
		{
			AfxMessageBox(_T("��ȡ����KEY�ɹ�"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	AfxMessageBox(_T("��ֹɾ��KeyFile��"));
	return;
	if (AfxMessageBox(_T("��ֹɾ��KeyFile,�Ƿ�ǿ��ɾ����"), MB_YESNO) == IDNO)
	{
		return;
	}

	if (m_ConaxPersonalize.DeleteJobFile())
	{
		AfxMessageBox(_T("ɾ���ɹ�"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	return ;

	CString strChipID;

	GetDlgItemText(IDC_CHIPID, strChipID);

	if (strChipID.GetLength() == 0)
	{
		AfxMessageBox(_T("jobid��chipid����Ϊ��"));
	}
	else
	{
		unsigned int chipid = atoi(strChipID.GetBuffer(0));
		unsigned char vscData[64*1024] = { 0 };
		unsigned int unDataLen = sizeof(vscData);

		if (m_ConaxPersonalize.RequestVSCData(vscData, &unDataLen))
		{
			AfxMessageBox(_T("��ȡVSC DATA �ɹ�"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString str;
	FILE *fp = NULL;

	GetDlgItem(IDC_MFCEDITBROWSE1)->GetWindowTextA(str);

	if (str.GetLength() == 0)
	{
		AfxMessageBox(_T("��ѡ��ǰreturnfile��returnfile����Ϊ�գ�"));
		return;
	}

	fopen_s(&fp, str.GetBuffer(0), "rb");
	if (fp == NULL)
	{
		AfxMessageBox(_T("�޷�����ѡ�ļ���"));
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
		AfxMessageBox(_T("IP��PORT����Ϊ��"));
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
		AfxMessageBox(_T("������������ɹ�, ��ѡ�񱣴�·����"));

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

			reallyWriteLen = fwrite(gVSCReport, 1, unLen, fp); /* д��struct�ļ�*/

			PrintfLog("reallyWriteLen:%d unLen:%d\r\n", reallyWriteLen, unLen);

			fclose(fp); /*�ر��ļ�*/
		}

	}
	else
	{
		AfxMessageBox(_T("������������ʧ�ܣ�"));
	}
}
