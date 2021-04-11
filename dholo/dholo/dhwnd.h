#pragma once

#include "dhogl.h"

class DHWnd : public CWnd
{
public:
	DHWnd();
	virtual ~DHWnd();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpcst);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

private:
	void InitOpenGL();
	void InitSpaceCord();
	void Render();

private:
	gpgpu::ogl::OGL m_ogl;

	PIXELFORMATDESCRIPTOR m_Desc;
	HGLRC m_hRC;
	PAINTSTRUCT m_Paint;
	CDC* m_pDC;

	float m_CubeCoord[4][2];
	float m_TexCoord[4][2];
};

