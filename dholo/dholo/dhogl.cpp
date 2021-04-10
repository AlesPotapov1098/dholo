#include "stdafx.h"
#include "dhogl.h"

namespace gpgpu
{
	namespace ogl
	{
		OGL::OGL()
		{
			m_hDC = nullptr;
			m_hRC = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));

			m_CubeCoord[0][0] = -0.5f;	m_TexCoord[0][0] = 0.0f;
			m_CubeCoord[0][1] = -0.5f;	m_TexCoord[0][1] = 0.0f;

			m_CubeCoord[1][0] = 0.5f;	m_TexCoord[1][0] = 1.0f;
			m_CubeCoord[1][1] = -0.5f;	m_TexCoord[1][1] = 0.0f;

			m_CubeCoord[2][0] = 0.5f;	m_TexCoord[2][0] = 1.0f;
			m_CubeCoord[2][1] = 0.5f;	m_TexCoord[2][1] = 1.0f;

			m_CubeCoord[3][0] = -0.5f;	m_TexCoord[3][0] = 0.0f;
			m_CubeCoord[3][1] = 0.5f;	m_TexCoord[3][1] = 1.0f;

			WIDTH = 1200;
			HEIGHT = 800;
		}

		OGL::~OGL()
		{
			m_hDC = nullptr;
			m_hRC = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		void OGL::InitOpenGL(HDC dc)
		{
			m_hDC = dc;

			m_Desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			m_Desc.nVersion = 1;
			m_Desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			m_Desc.iPixelType = PFD_TYPE_RGBA;
			m_Desc.cColorBits = 24;
			m_Desc.cRedBits = 0;
			m_Desc.cRedShift = 0;
			m_Desc.cGreenBits = 0;
			m_Desc.cGreenShift = 0;
			m_Desc.cBlueBits = 0;
			m_Desc.cBlueShift = 0;
			m_Desc.cAlphaBits = 0;
			m_Desc.cAlphaShift = 0;
			m_Desc.cAccumBits = 0;
			m_Desc.cAccumRedBits = 0;
			m_Desc.cAccumGreenBits = 0;
			m_Desc.cAccumBlueBits = 0;
			m_Desc.cAccumAlphaBits = 0;
			m_Desc.cDepthBits = 32;
			m_Desc.cStencilBits = 0;
			m_Desc.cAuxBuffers = 0;
			m_Desc.iLayerType = PFD_MAIN_PLANE;
			m_Desc.bReserved = 0;
			m_Desc.dwLayerMask = 0;
			m_Desc.dwVisibleMask = 0;

			int pf = ChoosePixelFormat(m_hDC, &m_Desc);
			if (!SetPixelFormat(m_hDC, pf, &m_Desc))
				return;

			m_hRC = wglCreateContext(m_hDC);
			if (!m_hRC)
				return;

			if (!wglMakeCurrent(m_hDC, m_hRC))
				return;
		}

		bool OGL::IsActive()
		{
			if (m_hRC == nullptr)
				return false;

			return true;
		}

		void OGL::Render(CDC* dc)
		{
			if (dc == nullptr)
				return;

			wglMakeCurrent(dc->GetSafeHdc(), m_hRC);

			glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glBegin(GL_QUADS);
				glVertex2f(-0.5f, 0.5f);
				glVertex2f(0.5f, 0.5f);
				glVertex2f(0.5f, -0.5f);
				glVertex2f(-0.5f, -0.5f);
			glEnd();
			
			glFinish();
			SwapBuffers(wglGetCurrentDC());
		}

		void OGL::InitTexture(CString pathImage)
		{
			//HRESULT res = m_Image.Load(pathImage);
			//if (res != S_OK)
			//	return;
			//
			//BOOL isAttached = m_Bitmap.Attach((HBITMAP)m_Image);
			//if (!isAttached)
			//	return;
			//
			//int isLoadBitmap = m_Bitmap.GetBitmap(&m_hBitmap);
			//if (!isLoadBitmap)
			//	return;

			std::size_t size = WIDTH * HEIGHT * 4;
			pixels = new GLuint[size];
			if (pixels == nullptr)
				return;

			for (int i = 0; i < size;)
			{
				pixels[i++] = 0;
				pixels[i++] = 0;
				pixels[i++] = 0;
				pixels[i++] = 0;
			}

			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			//glEnable(GL_TEXTURE_2D);
			//glGenTextures(1, &m_Texture);
			//glBindTexture(GL_TEXTURE_2D, m_Texture);
			//
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_hBitmap.bmWidth, m_hBitmap.bmHeight, 0, GL_RGB8, GL_UNSIGNED_BYTE, m_hBitmap.bmBits);
		}

		void OGL::ReleaseOpenGL()
		{
			//wglDeleteContext(m_hRC);
		}
	}
}
