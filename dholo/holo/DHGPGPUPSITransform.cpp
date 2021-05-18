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

		DHGPGPUPSITransform::DHGPGPUPSITransform(GLuint* tex, const PSIStruct& psi)
		{
			m_PSISettings = psi;
			m_ProgramPath = "PSI.cl";
			m_KernelName = "PSI";
			m_RenderTex = tex;
		}

		DHGPGPUPSITransform::~DHGPGPUPSITransform()
		{
			DHGPGPUTransform::~DHGPGPUTransform();
		}

		void DHGPGPUPSITransform::Init(const DHOCLHost& host, HDC dc, HGLRC rc)
		{
			DHGPGPUTransform::Init(host, dc, rc);
			
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

			glEnable(GL_TEXTURE_2D);
			glGenTextures(4, m_Textures);

			cl_int error_code = CL_SUCCESS;

			for (int i = 0; i < 5; i++)
			{
				glBindTexture(GL_TEXTURE_2D, i == 4 ? *m_RenderTex : m_Textures[i]);

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
					CL_MEM_READ_WRITE,
					GL_TEXTURE_2D,
					0, i == 4 ? *m_RenderTex : m_Textures[i],
					&error_code);

				if (error_code != CL_SUCCESS)
					throw dholo::exp::DHGPGPUExp(error_code);
			}
		}

		void DHGPGPUPSITransform::Calculate(int global_w, int global_h, int local_w, int local_h)
		{
			DHGPGPUTransform::Calculate(0, 0, 0, 0);

			cl_float4 S, C;
			cl_float B;

			C.x = std::cosf(m_PSISettings.m_Phases[1]) - std::cosf(m_PSISettings.m_Phases[3]);
			C.y = std::cosf(m_PSISettings.m_Phases[2]) - std::cosf(m_PSISettings.m_Phases[0]);
			C.z = std::cosf(m_PSISettings.m_Phases[3]) - std::cosf(m_PSISettings.m_Phases[1]);
			C.w = std::cosf(m_PSISettings.m_Phases[0]) - std::cosf(m_PSISettings.m_Phases[2]);

			S.x = std::sinf(m_PSISettings.m_Phases[1]) - std::sinf(m_PSISettings.m_Phases[3]);
			S.y = std::sinf(m_PSISettings.m_Phases[2]) - std::sinf(m_PSISettings.m_Phases[0]);
			S.z = std::sinf(m_PSISettings.m_Phases[3]) - std::sinf(m_PSISettings.m_Phases[1]);
			S.w = std::sinf(m_PSISettings.m_Phases[0]) - std::sinf(m_PSISettings.m_Phases[2]);

			B = C.x * std::sinf(m_PSISettings.m_Phases[0]) + C.y * std::sinf(m_PSISettings.m_Phases[1]) +
				C.z * std::sinf(m_PSISettings.m_Phases[2]) + C.w * std::sinf(m_PSISettings.m_Phases[3]);

			B = std::fabs(B);

			cl_int error = clEnqueueAcquireGLObjects(m_CommandQueue, 5, m_Mem, 0, 0, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error |= clSetKernelArg(m_Kernel, 0, sizeof(m_Mem[0]), &m_Mem[0]);
			error |= clSetKernelArg(m_Kernel, 1, sizeof(m_Mem[1]), &m_Mem[1]);
			error |= clSetKernelArg(m_Kernel, 2, sizeof(m_Mem[2]), &m_Mem[2]);
			error |= clSetKernelArg(m_Kernel, 3, sizeof(m_Mem[3]), &m_Mem[3]);
			error |= clSetKernelArg(m_Kernel, 4, sizeof(cl_float4), &S);
			error |= clSetKernelArg(m_Kernel, 5, sizeof(cl_float4), &C);
			error |= clSetKernelArg(m_Kernel, 6, sizeof(cl_float), &B);
			error |= clSetKernelArg(m_Kernel, 7, sizeof(m_Mem[4]), &m_Mem[4]);

			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			const std::size_t global_size[2] = { m_Images[0].GetWidth(), m_Images[0].GetHeight()};
			const std::size_t local_size[2] = { 1, 1 };

			error = clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, 0, global_size, local_size, 0, NULL, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error = clEnqueueReleaseGLObjects(m_CommandQueue, 5, m_Mem, 0, 0, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error = clFinish(m_CommandQueue);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);
		}

		void DHGPGPUPSITransform::Release()
		{
			DHGPGPUTransform::Release();
		}
	}
}