
// UpgradeTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUpgradeToolApp: 
// �йش����ʵ�֣������ UpgradeTool.cpp
//

class CUpgradeToolApp : public CWinApp
{
public:
	CUpgradeToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUpgradeToolApp theApp;