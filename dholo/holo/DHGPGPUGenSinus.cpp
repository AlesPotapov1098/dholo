#include "pch.h"
#include "DHGPGPUGenSinus.h"

namespace dholo
{
	namespace gpgpu
	{
		DHGPGPUGenSinus::DHGPGPUGenSinus(GLuint* tex, int width, int height, float ampl, float phi, int T)
		{
			DHGPGPUTransform::DHGPGPUTransform();

			m_width = width;
			m_height = height;
			m_amp = ampl;
			m_phase = phi;
			m_T = T;
			m_SinTexture = tex;
		}

		DHGPGPUGenSinus::~DHGPGPUGenSinus()
		{
			DHGPGPUTransform::~DHGPGPUTransform();
		}

		void DHGPGPUGenSinus::Calculate(int global_w, int global_h, int local_w, int local_h)
		{
			float* sinus = new float[m_height];
			float coeff = 2 * CL_M_PI * m_T / m_height;
			for (int n = 0; n < m_height; n++)
				sinus[n] = m_amp * ((std::sinf(coeff * n + m_phase) + 1) / 2);

			float* pixels = new float[m_width * m_height * 3];
			int k = 0;
			for (int i = 0; i < m_width; i++)
				for (int j = 0; j < m_height; j++)
				{
					pixels[k++] = sinus[j];
					pixels[k++] = sinus[j];
					pixels[k++] = sinus[j];
				}

			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, m_SinTexture);
			glBindTexture(GL_TEXTURE_2D, *m_SinTexture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_FLOAT, pixels);

			delete[] pixels;
			delete[] sinus;
		}
	}
}