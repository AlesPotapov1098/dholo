#pragma once

#include "dhogl.h"
#include "dhimgloader.h"

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
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
	void InitOpenGL();
	void LoadTexture(const CString& path);
	void Render();

private:
	gpgpu::ogl::OGL m_ogl;

	PIXELFORMATDESCRIPTOR m_Desc;
	HGLRC m_hRC;
	PAINTSTRUCT m_Paint;
	CDC* m_pDC;

	GLuint m_OpenGLTexture;
	int WIDTH = 1200;
	int HEIGHT = 800;
	unsigned char* data;

	DHImgLoader m_ImgLoader;
};

