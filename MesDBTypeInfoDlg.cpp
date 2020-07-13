// MesDBTypeInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpgradeTool.h"
#include "MesDBTypeInfoDlg.h"
#include "afxdialogex.h"
#include "DataManage/FactoryDBServerData/FactoryDBServerData.h"


// MesDBTypeInfoDlg 对话框

IMPLEMENT_DYNAMIC(MesDBTypeInfoDlg, CDialogEx)

MesDBTypeInfoDlg::MesDBTypeInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MesDBTypeInfoDlg::IDD, pParent)
{
	memset(m_acTypeName, 0, sizeof(m_acTypeName));
}

MesDBTypeInfoDlg::~MesDBTypeInfoDlg()
{
}

void MesDBTypeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL MesDBTypeInfoDlg::OnInitDialog(void)
{
	CDialogEx::OnInitDialog();

	CListCtrl *pCListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_FILED);

	CRect rect;

	pCListCtrl->GetClientRect(&rect);
	pCListCtrl->SetExtendedStyle(pCListCtrl->GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	pCListCtrl->InsertColumn(0, _T("数据库字段名"), LVCFMT_LEFT, rect.Width() / 2, 0);
	pCListCtrl->InsertColumn(1, _T("实际字段名"), LVCFMT_LEFT, rect.Width() / 2, 1);

	ShowMesDBInfo(pCListCtrl);

	return FALSE;
}

BEGIN_MESSAGE_MAP(MesDBTypeInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// MesDBTypeInfoDlg 消息处理程序

void MesDBTypeInfoDlg::SetDBTypeAndTypeName(unsigned int unDBType, char *pTypeName, int nLen)
{
	m_unDBType = unDBType;

	memcpy(m_acTypeName, pTypeName, nLen);
}

typedef struct
{
	unsigned int m_unID;
	char m_acDBName[32];
	char m_acActName[32];
}DBFiled_s;

static DBFiled_s g_stDBFiled[] = {
	{ E_CHIP_RT, "ChipId", "CHIPID号" },
	{ E_SN_RT, "SNSTB", "SN号" },
	{ E_MAC_RT, "MacEth", "MAC号" },
	{ E_CDSN_RT, "SnGD", "CDSN" },
	{ E_PRIVATE_RT, "Option1", "私有数据" },
	{ E_PIN_RT, "Option2", "TV PIN" },
	{ E_DIGCHIP_RT, "Option3", "十进制CHIPID" },
	{ E_JOBID_RT, "Option7", "JOB ID" },
	{ E_VSCID_RT, "Option8", "VSC ID" },
	{ E_VALID_RT, "CodeContent", "VALID NUM" },
	{ E_TEMPSN_RT, "SnTemp", "临时批号" },
	{ E_CA_SERIAL_NUM, "Option9", "CA SERIAL NUM" },
	{ E_CARD_ID, "Option10", "CARD ID" },
	{ E_CARD_SN, "SnCard", "CARD SN" },
	{ E_TSC_DATA, "RandNo", "TSC Data" }
};

void MesDBTypeInfoDlg::ShowMesDBInfo(CListCtrl *pCListCtrl)
{
	int nIndex = 0;
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(m_acTypeName);

	for (int ii = 0; ii < sizeof(g_stDBFiled) / sizeof(g_stDBFiled[0]); ii++)
	{
		if (m_unDBType & g_stDBFiled[ii].m_unID)
		{
			pCListCtrl->InsertItem(nIndex, g_stDBFiled[ii].m_acDBName);
			pCListCtrl->SetItemText(nIndex, 1, g_stDBFiled[ii].m_acActName);
			nIndex++;
		}
	}
}