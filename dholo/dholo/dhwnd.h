#pragma once

#include "dhogl.h"
#include "dhimgloader.h"

class DHWnd : public CWnd
{
public:
	DHWnd();
	virtual ~DHWnd();

	void OpenDlg();
	void LoadTexture(const CStringA& path);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpcst);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
	DHOGLRender mogl;

	DHImgLoader m_ImgLoader;

	PAINTSTRUCT m_Paint;
	CDC* m_pDC;
	RECT m_Rect;
};

