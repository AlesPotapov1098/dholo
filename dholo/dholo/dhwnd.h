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
	void InitOpenGL();
	void Render();

private:
	gpgpu::ogl::OGL m_ogl;

	PIXELFORMATDESCRIPTOR m_Desc;
	HGLRC m_hRC;
	PAINTSTRUCT m_Paint;
	CDC* m_pDC;

	GLuint m_OpenGLTexture;

	GLfloat m_X;
	GLfloat m_Y;

	RECT m_Rect;
	
	DHImgLoader m_ImgLoader;
};

