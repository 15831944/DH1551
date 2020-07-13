#pragma once


// MesDBTypeInfoDlg �Ի���

class MesDBTypeInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MesDBTypeInfoDlg)

public:
	MesDBTypeInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MesDBTypeInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DB_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog(void);

	void SetDBTypeAndTypeName(unsigned int unDBType, char *pTypeName, int nLen);

	void ShowMesDBInfo(CListCtrl *pCListCtrl);

private:
	unsigned int m_unDBType;
	char m_acTypeName[128];
};
