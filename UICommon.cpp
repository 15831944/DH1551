#include "UICommon.h"

UICommon::UICommon()
{
}

UICommon::~UICommon()
{
}

BOOL UICommon::CheckComUsed(char *pComName, int nLen)
{
	BOOL bEnable = FALSE;
	COMMCONFIG cfg;
	DWORD  t;
	HANDLE hHandle;
	char acOpenCom[16] = { 0 };

	if (nLen > 4)
	{
		sprintf_s(acOpenCom, sizeof(acOpenCom), "\\\\.\\%s", pComName);
	}
	else
	{
		sprintf_s(acOpenCom, sizeof(acOpenCom), "%s", pComName);
	}

	hHandle = CreateFileA(acOpenCom, GENERIC_READ | GENERIC_WRITE, 0,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			NULL);

	if (hHandle)
	{
		t = sizeof(cfg);
		if (GetCommConfig(hHandle, &cfg, &t))
		{
			bEnable = TRUE;
		}
		CloseHandle(hHandle);
	}

	return bEnable;
}

int UICommon::CreatPrivateFont(CFont * pFont, int nHeight)
{
	int nRet = 0;

	if (pFont != NULL)
	{
		pFont->CreateFont(nHeight, // nHeight

			0, // nWidth

			0, // nEscapement

			0, // nOrientation

			FW_EXTRABOLD, // nWeight

			FALSE, // bItalic

			FALSE, // bUnderline

			0, // cStrikeOut

			ANSI_CHARSET, // nCharSet

			OUT_DEFAULT_PRECIS, // nOutPrecision

			CLIP_DEFAULT_PRECIS, // nClipPrecision

			DEFAULT_QUALITY, // nQuality

			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily

			_T("Arial")); // lpszFac 

		nRet = 1;
	}

	return nRet;
}

void UICommon::SetComboBoxSize(CComboBox *pbox, UINT LinesToDisplay)
{
	ASSERT(IsWindow(*pbox)); // Window must exist or SetWindowPos won't work 

	CRect cbSize; // current size of combo pbox 
	int Height; // new height for drop-down portion of combo pbox 

	pbox->GetClientRect(cbSize);
	Height = pbox->GetItemHeight(-1); // start with size of the edit-pbox portion 
	Height += pbox->GetItemHeight(0) * LinesToDisplay; // add height of lines of text 

	// Note: The use of SM_CYEDGE assumes that we're using Windows '95 
	// Now add on the height of the border of the edit pbox 
	Height += GetSystemMetrics(SM_CYEDGE) * 2; // top & bottom edges 

	// The height of the border of the drop-down pbox 
	Height += GetSystemMetrics(SM_CYEDGE) * 2; // top & bottom edges 

	// now set the size of the window 
	pbox->SetWindowPos(NULL, // not relative to any other windows 
		0, 0, // TopLeft corner doesn't change 
		cbSize.right, Height, // existing width, new height 
		SWP_NOMOVE | SWP_NOZORDER // don't move pbox or change z-ordering. 
		);
}