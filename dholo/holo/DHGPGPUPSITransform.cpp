#include "pch.h"
#include "DHGPGPUPSITransform.h"

namespace dholo
{
	namespace gpgpu
	{
		DHGPGPUPSITransform::DHGPGPUPSITransform()
		{
			DHGPGPUTransform::DHGPGPUTransform();
		}

		DHGPGPUPSITransform::DHGPGPUPSITransform(const std::vector<dholo::img::DHImgLoader>& imgs, float phases[4], float B)
		{
			m_B = B;

			for (int i = 0; i < 4; i++)
			{
				m_Images[i] = imgs[i];
				m_Phases[i] = phases[i];
			}
		}

		DHGPGPUPSITransform::~DHGPGPUPSITransform()
		{
			DHGPGPUTransform::~DHGPGPUTransform();
		}

		void DHGPGPUPSITransform::Init(const CDC& dc, const DHOCLHost& host)
		{
			DHGPGPUTransform::Init(dc, host);
			InitOpenCL(host);
		}

		void DHGPGPUPSITransform::SetImages(const std::vector<dholo::img::DHImgLoader>& imgs)
		{
			m_Images = imgs;
		}

		void DHGPGPUPSITransform::GenerateTexture()
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(5, m_Textures);

			cl_int error_code = CL_SUCCESS;

			for (int i = 0; i < 5; i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_Textures[i]);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexImage2D(
					GL_TEXTURE_2D, 0, 
					m_Images[i].GetChannels() == 3 ? GL_RGB : GL_RGBA,
					m_Images[i].GetWidth(),
					m_Images[i].GetHeight(),
					0, 
					m_Images[i].GetChannels() == 3 ? GL_RGB : GL_RGBA, 
					GL_FLOAT,
					m_Images[i].GetPixelsData());

				m_Mem[i] = clCreateFromGLTexture(
					m_Context,
					CL_MEM_READ_ONLY,
					GL_TEXTURE_2D,
					0, m_Textures[i],
					&error_code);

				if (error_code != CL_SUCCESS)
					/// TODO: обработка ошибок
					return;
			}
		}

		void DHGPGPUPSITransform::Calculate()
		{
		}

		void DHGPGPUPSITransform::Release()
		{
		}

		void DHGPGPUPSITransform::RenderScene()
		{
		}
	}
}