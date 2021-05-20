
// holo.h: основной файл заголовка для приложения holo
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы

#include <vector>

// DHApp:
// Сведения о реализации этого класса: holo.cpp
//

class DHApp : public CWinAppEx
{
public:
	DHApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	void SelectImage(const std::vector<CStringA>& imgPath);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern DHApp theApp;
