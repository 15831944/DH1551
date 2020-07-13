#ifndef UI_COMMON_H_
#define UI_COMMON_H_

#include <afxwin.h>

class UICommon
{
public:
	UICommon();
	~UICommon();

public:
	BOOL CheckComUsed(char *pComName, int nLen);
	int CreatPrivateFont(CFont * pFont, int nHeight);
	void SetComboBoxSize(CComboBox *pbox, UINT LinesToDisplay);

private:
	int CheckComName(char *pComName, int nLen);
};

#endif/*UI_COMMOM_H_*/