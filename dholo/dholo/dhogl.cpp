#include "stdafx.h"
#include "dhogl.h"

namespace gpgpu
{
	namespace ogl
	{
		OGL::OGL()
		{
			m_hRC = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		OGL::~OGL()
		{
			m_hRC = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		void OGL::InitOpenGL()
		{
			m_Desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			m_Desc.nVersion = 1;
			m_Desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			m_Desc.iPixelType = PFD_TYPE_RGBA;
			m_Desc.cColorBits = 64;
			m_Desc.cDepthBits = 32;
			m_Desc.iLayerType = PFD_MAIN_PLANE;
		}

		bool OGL::IsActive()
		{
			if (m_hRC == nullptr)
				return false;

			return true;
		}

		void OGL::Render(CDC* dc, CRect* rect)
		{
			if (dc == nullptr || rect == nullptr)
				return;

			m_Rect = rect;

			if (!SetPixelFormat(dc->m_hDC, 
				ChoosePixelFormat(dc->m_hDC, &m_Desc),
				&m_Desc))
				return;

			m_hRC = wglCreateContext(dc->m_hDC);
			if (!m_hRC)
				return;

			if (!wglMakeCurrent(dc->m_hDC, m_hRC))
				return;

			wglMakeCurrent(dc->m_hDC, m_hRC);

			glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			DrawTexture();

			SwapBuffers(dc->m_hDC);
		}

		void OGL::InitTexture(const CStringA& pathImage)
		{
			m_ImgLoader.Load(pathImage);
		}

		void OGL::DrawTexture()
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &m_Texture);
			glBindTexture(GL_TEXTURE_2D, m_Texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (m_ImgLoader.GetChannels() == 3)
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGB,
					m_ImgLoader.GetWidth(),
					m_ImgLoader.GetHeight(),
					0, GL_RGB, GL_UNSIGNED_BYTE,
					m_ImgLoader.GetPixelsData());
			else if (m_ImgLoader.GetChannels() == 4)
			{
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGBA,
					m_ImgLoader.GetWidth(),
					m_ImgLoader.GetHeight(),
					0, GL_RGBA, GL_UNSIGNED_BYTE,
					m_ImgLoader.GetPixelsData());

				m_X = (float)m_ImgLoader.GetWidth() / (float)(m_Rect->Width());
				m_Y = (float)m_ImgLoader.GetHeight() / (float)(m_Rect->Height());

				m_X = m_X >= 1.0f ? 1.0f : m_X;
				m_Y = m_Y >= 1.0f ? 1.0f : m_Y;
			}
			else
			{
				m_X = 0.95f;
				m_Y = 0.95f;
			}

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(-m_X, -m_Y);
				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(-m_X, m_Y);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(m_X, m_Y);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(m_X, -m_Y);
			glEnd();
		}

		void OGL::ReleaseOpenGL()
		{
			wglDeleteContext(m_hRC);
		}
	}
}
