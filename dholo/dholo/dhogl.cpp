#include "stdafx.h"
#include "dhogl.h"

namespace dholo
{
	namespace ogl
	{
		DHOGLRender::DHOGLRender()
		{
			m_hRC = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));

			m_Desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			m_Desc.nVersion = 1;
			m_Desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			m_Desc.iPixelType = PFD_TYPE_RGBA;
			m_Desc.cColorBits = 64;
			m_Desc.cDepthBits = 32;
			m_Desc.iLayerType = PFD_MAIN_PLANE;
		}

		DHOGLRender::~DHOGLRender()
		{
			m_hRC = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		void DHOGLRender::Init(const CDC & dc)
		{
			if (dc == nullptr)
				return;

			if (!SetPixelFormat(dc.m_hDC,
				ChoosePixelFormat(dc.m_hDC, &m_Desc),
				&m_Desc))
				return;

			m_hRC = wglCreateContext(dc.m_hDC);
			if (!m_hRC)
				return;

			if (!wglMakeCurrent(dc.m_hDC, m_hRC))
				return;

			wglMakeCurrent(dc.m_hDC, m_hRC);
		}

		void DHOGLRender::LoadImg(const dholo::img::DHImgLoader & imgldr, const CRect& rect)
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &m_Texture);
			glBindTexture(GL_TEXTURE_2D, m_Texture);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (imgldr.GetChannels() == 3)
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGB,
					imgldr.GetWidth(),
					imgldr.GetHeight(),
					0, GL_RGB, GL_FLOAT,
					imgldr.GetPixelsData());
			else if (imgldr.GetChannels() == 4)
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGBA,
					imgldr.GetWidth(),
					imgldr.GetHeight(),
					0, GL_RGBA, GL_FLOAT,
					imgldr.GetPixelsData());

			m_X = (float)imgldr.GetWidth() / (float)(rect.Width());
			m_Y = (float)imgldr.GetHeight() / (float)(rect.Height());

			m_X = m_X >= 1.0f ? 1.0f : m_X;
			m_Y = m_Y >= 1.0f ? 1.0f : m_Y;

			glFinish();
		}

		void DHOGLRender::Draw()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindTexture(GL_TEXTURE_2D, m_Texture);

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

			glFinish();
		}

		GLuint DHOGLRender::GetTexture() const
		{
			return m_Texture;
		}
	}
}
