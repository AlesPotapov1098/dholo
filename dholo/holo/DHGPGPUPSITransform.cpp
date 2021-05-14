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

		DHGPGPUPSITransform::DHGPGPUPSITransform(const PSIStruct& psi)
		{
			m_PSISettings = psi;
		}

		DHGPGPUPSITransform::~DHGPGPUPSITransform()
		{
			DHGPGPUTransform::~DHGPGPUTransform();
		}

		void DHGPGPUPSITransform::Init(const CDC& dc, const DHOCLHost& host)
		{
			DHGPGPUTransform::Init(dc, host);
			InitOpenCL(host);
			
			for (int i = 0; i < 4; i++)
				if (!m_Images[i].Load((CStringA)m_PSISettings.m_ImgNames[i]))
					throw dholo::exp::DHAppExp("Can't load image");

			int width = m_Images[0].GetWidth();
			int height = m_Images[0].GetHeight();
			int channels = m_Images[0].GetChannels();

			for (int i = 1; i < 4; i++)
				if (width != m_Images[i].GetWidth() ||
					height != m_Images[i].GetHeight() ||
					channels != m_Images[i].GetChannels())
					throw dholo::exp::DHAppExp("Some or any image parametrs not equal!!!");

			m_Images[4].GenerateImage(width, height, channels);
		}

		void DHGPGPUPSITransform::GenerateTexture()
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(5, m_Textures);

			cl_int error_code = CL_SUCCESS;

			for (int i = 0; i < 4; i++)
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
			DHGPGPUTransform::Release();
		}

		void DHGPGPUPSITransform::RenderScene()
		{
		}
	}
}