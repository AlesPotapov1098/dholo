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
			m_Desc.cColorBits = 32;
			m_Desc.cDepthBits = 24;
			m_Desc.iLayerType = PFD_MAIN_PLANE;

			m_Texture = nullptr;
		}

		DHOGLRender::DHOGLRender(const CDC& dc)
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

			m_Texture = nullptr;

			Init(dc);
		}

		DHOGLRender::~DHOGLRender()
		{
			m_hRC = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));
			glFinish();
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hRC);
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

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}

		void DHOGLRender::LoadImg(const dholo::img::DHImgLoader & imgldr, const CRect& rect)
		{
			glEnable(GL_TEXTURE_2D);

			if(!m_Texture)
				m_Texture = new GLuint;

			glGenTextures(1, m_Texture);
			glBindTexture(GL_TEXTURE_2D, *m_Texture);

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
		}

		void DHOGLRender::LoadImg(const std::vector<dholo::img::DHImgLoader>& imgldr)
		{
			glEnable(GL_TEXTURE_2D);

			m_Cnt = imgldr.size();
			m_Texture = new GLuint[m_Cnt];
			glGenTextures(m_Cnt, m_Texture);

			for (int i = 0; i < m_Cnt; i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_Texture[i]);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if (imgldr[i].GetChannels() == 3)
					glTexImage2D(
						GL_TEXTURE_2D, 0, GL_RGB,
						imgldr[i].GetWidth(),
						imgldr[i].GetHeight(),
						0, GL_RGB, GL_FLOAT,
						imgldr[i].GetPixelsData());
				else if (imgldr[i].GetChannels() == 4)
					glTexImage2D(
						GL_TEXTURE_2D, 0, GL_RGBA,
						imgldr[i].GetWidth(),
						imgldr[i].GetHeight(),
						0, GL_RGBA, GL_FLOAT,
						imgldr[i].GetPixelsData());
			}
		}

		void DHOGLRender::LoadImg(const test::DHTest & test)
		{
			glEnable(GL_TEXTURE_2D);

			m_Cnt = 5;
			m_Texture = new GLuint[m_Cnt];
			glGenTextures(m_Cnt, m_Texture);

			for (int i = 0; i < m_Cnt; i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_Texture[i]);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
					test.GetWidth(), test.GetHeight(), 
					0, GL_RGB, GL_FLOAT, 
					test.GetTestExmpl(i));
			}
		}

		void DHOGLRender::LoadTexture(const std::vector<dholo::img::DHImgLoader>& imgldr, const CRect& rect)
		{
			glEnable(GL_TEXTURE_2D);

			m_Cnt = imgldr.size() + 1;
			m_Texture = new GLuint[m_Cnt];
			glGenTextures(m_Cnt, m_Texture);

			for (int i = 0; i < m_Cnt; i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_Texture[i]);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if (imgldr[i].GetChannels() == 3)
					glTexImage2D(
						GL_TEXTURE_2D, 0, GL_RGB,
						imgldr[i].GetWidth(),
						imgldr[i].GetHeight(),
						0, GL_RGB, GL_FLOAT,
						imgldr[i].GetPixelsData());
				else if (imgldr[i].GetChannels() == 4)
					glTexImage2D(
						GL_TEXTURE_2D, 0, GL_RGBA,
						imgldr[i].GetWidth(),
						imgldr[i].GetHeight(),
						0, GL_RGBA, GL_FLOAT,
						imgldr[i].GetPixelsData());
			}

			m_X = (float)imgldr[0].GetWidth() / (float)(rect.Width());
			m_Y = (float)imgldr[0].GetHeight() / (float)(rect.Height());

			m_X = m_X >= 1.0f ? 1.0f : m_X;
			m_Y = m_Y >= 1.0f ? 1.0f : m_Y;
		}

		void DHOGLRender::Draw()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!m_Texture)
			{
				glFinish();
				return;
			}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_Texture[m_Cnt - 1]);

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

		void DHOGLRender::Draw(unsigned int index)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!m_Texture)
			{
				glFinish();
				return;
			}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_Texture[index]);

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(-1.0f, 1.0f);

				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(1.0f, 1.0f);

				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(1.0f, -1.0f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(-1.0f, -1.0f);
			glEnd();
		}

		void DHOGLRender::DrawAll()
		{
			if (!m_Texture)
				return;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindTexture(GL_TEXTURE_2D, m_Texture[0]);
			glBegin(GL_QUADS);

				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(-0.9f, 0.9f);

				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(-0.1f, 0.9f);

				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(-0.1f, 0.1f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(-0.9f, 0.1f);

			glEnd();

			glBindTexture(GL_TEXTURE_2D, m_Texture[1]);
			glBegin(GL_QUADS);

				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0.1f, 0.9f);

				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(0.9f, 0.9f);

				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(0.9f, 0.1f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(0.1f, 0.1f);

			glEnd();

			glBindTexture(GL_TEXTURE_2D, m_Texture[2]);
			glBegin(GL_QUADS);

				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(-0.9f, -0.1f);

				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(-0.1f, -0.1f);

				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(-0.1f, -0.9f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(-0.9f, -0.9f);

			glEnd();

			glBindTexture(GL_TEXTURE_2D, m_Texture[3]);
			glBegin(GL_QUADS);

				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0.1f, -0.1f);
				
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(0.9f, -0.1f);
				
				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(0.9f, -0.9f);
				
				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(0.1f, -0.9f);

			glEnd();
		}

		GLuint DHOGLRender::GetTexture() const
		{
			return *m_Texture;
		}

		GLuint DHOGLRender::GetTexture(unsigned int index) const
		{
			if (index >= m_Cnt)
				return 0;

			return m_Texture[index];
		}


	}
}
