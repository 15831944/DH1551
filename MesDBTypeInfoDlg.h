#pragma once


// MesDBTypeInfoDlg 对话框

class MesDBTypeInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MesDBTypeInfoDlg)

public:
	MesDBTypeInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MesDBTypeInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DB_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog(void);

	void SetDBTypeAndTypeName(unsigned int unDBType, char *pTypeName, int nLen);

	void ShowMesDBInfo(CListCtrl *pCListCtrl);

private:
	unsigned int m_unDBType;
	char m_acTypeName[128];
};
