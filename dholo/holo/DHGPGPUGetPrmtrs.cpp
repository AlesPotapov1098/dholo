#include "pch.h"
#include "DHGPGPUGetPrmtrs.h"

namespace dholo
{
	namespace gpgpu
	{
		DHGPGPUGetPrmtrs::DHGPGPUGetPrmtrs(const dholo::img::DHImgLoader& img)
		{
			m_ImgLdr = img;
		}

		void DHGPGPUGetPrmtrs::Init(const CDC& dc, const DHOCLHost& host)
		{
			DHGPGPUShowImg::Init(dc, host);
			InitOpenCL();
		}

		void DHGPGPUGetPrmtrs::GenerateTexture()
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &m_Texture);

			cl_int error_code = CL_SUCCESS;

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

			m_Mem = clCreateFromGLTexture(
				m_Context,
				CL_MEM_READ_WRITE,
				GL_TEXTURE_2D,
				0, m_Texture,
				&error_code);

			if (error_code != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error_code);
		}

		void DHGPGPUGetPrmtrs::Calculate()
		{
			cl_int error = clEnqueueAcquireGLObjects(m_CommandQueue, 1, &m_Mem, 0, 0, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error |= clSetKernelArg(m_Kernel, 0, sizeof(m_Mem), &m_Mem);

			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			const std::size_t global_size[2] = {m_ImgLdr.GetWidth(), m_ImgLdr.GetHeight() };
			const std::size_t local_size[2] = { 1, 1 };

			error = clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, 0, global_size, local_size, 0, NULL, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error = clEnqueueReleaseGLObjects(m_CommandQueue, 1, &m_Mem, 0, 0, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error = clFinish(m_CommandQueue);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);
		}

		void DHGPGPUGetPrmtrs::RenderScene()
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

		void DHGPGPUGetPrmtrs::Release()
		{
			if (m_Mem)
				clReleaseMemObject(m_Mem);

			DHGPGPUShowImg::Release();
		}
	}
}
