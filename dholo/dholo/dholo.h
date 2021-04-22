#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы

#include <vector>

class DHApp : public CWinAppEx
{
public:
	DHApp();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	void SelectImage(const std::vector<CStringA>& imgPath);
	void LoadImg(const CStringA& imgPath);
	void LoadImg(const std::vector<CStringA>& imgPaths);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern DHApp theApp;
