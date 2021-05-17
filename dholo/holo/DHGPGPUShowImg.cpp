#include "pch.h"
#include "DHGPGPUShowImg.h"

namespace dholo
{
	namespace gpgpu
	{
		DHGPGPUShowImg::DHGPGPUShowImg(const dholo::img::DHImgLoader& img)
		{
			m_ImgLdr = img;
		}

		DHGPGPUShowImg::~DHGPGPUShowImg()
		{
		}

		void DHGPGPUShowImg::GenerateTexture()
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &m_Texture);

			glBindTexture(GL_TEXTURE_2D, m_Texture);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(
				GL_TEXTURE_2D, 0,
				m_ImgLdr.GetChannels() == 3 ? GL_RGB : GL_RGBA,
				m_ImgLdr.GetWidth(),
				m_ImgLdr.GetHeight(),
				0,
				m_ImgLdr.GetChannels() == 3 ? GL_RGB : GL_RGBA,
				GL_FLOAT,
				m_ImgLdr.GetPixelsData());
		}

		void DHGPGPUShowImg::RenderScene()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_Texture);

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(-1.0f, -1.0f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(-1.0f, 1.0f);

				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(1.0f, 1.0f);

				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(1.0f, -1.0f);
			glEnd();

			SwapBuffers(m_hDC);
		}
	}
}
