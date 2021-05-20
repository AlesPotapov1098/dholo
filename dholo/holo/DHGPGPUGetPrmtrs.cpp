#include "pch.h"
#include "DHGPGPUGetPrmtrs.h"

namespace dholo
{
	namespace gpgpu
	{
		DHGPGPUGetPrmtrs::DHGPGPUGetPrmtrs(GLuint* tex, const img::DHImgLoader& imgLdr)
		{
			m_Tex = tex;
			m_InputImg = nullptr;
			m_ImgLdr = imgLdr;
		}

		void DHGPGPUGetPrmtrs::Init(const DHOCLHost& host, HDC dc, HGLRC rc)
		{
			DHGPGPUTransform::Init(host,dc, rc);

			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &m_InputTexture);
			glBindTexture(1, m_InputTexture);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(
				GL_TEXTURE_2D, 
				0, 
				m_ImgLdr.GetChannels() == 3 ? GL_RGB : GL_RGBA, 
				m_ImgLdr.GetWidth(), 
				m_ImgLdr.GetHeight(), 
				0, 
				m_ImgLdr.GetChannels() == 3 ? GL_RGB : GL_RGBA,
				GL_FLOAT, 
				m_ImgLdr.GetPixelsData());

			cl_int error_code = CL_SUCCESS;

			m_OutputImg = clCreateFromGLTexture(
				m_Context,
				CL_MEM_WRITE_ONLY,
				GL_TEXTURE_2D,
				0, *m_Tex,
				&error_code);

			if (error_code != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error_code);

			m_InputImg = clCreateFromGLTexture(
				m_Context,
				CL_MEM_READ_ONLY,
				GL_TEXTURE_2D,
				0, m_InputTexture,
				&error_code);
			
			if (error_code != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error_code);
		}

		void DHGPGPUGetPrmtrs::Calculate(int global_w, int global_h, int local_w, int local_h)
		{
			cl_int error = clEnqueueAcquireGLObjects(m_CommandQueue, 1, &m_InputImg, 0, 0, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error = clEnqueueAcquireGLObjects(m_CommandQueue, 1, &m_OutputImg, 0, 0, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error |= clSetKernelArg(m_Kernel, 0, sizeof(m_InputImg), &m_InputImg);
			error |= clSetKernelArg(m_Kernel, 1, sizeof(m_OutputImg), &m_OutputImg);

			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			const std::size_t global_size[2] = {global_w, global_h };
			const std::size_t local_size[2] = { local_w, local_h };

			error = clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, 0, global_size, local_size, 0, NULL, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error = clEnqueueReleaseGLObjects(m_CommandQueue, 1, &m_InputImg, 0, 0, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error = clFinish(m_CommandQueue);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);
		}

		void DHGPGPUGetPrmtrs::Release()
		{
			if (m_InputImg)
				clReleaseMemObject(m_InputImg);

			DHGPGPUTransform::Release();
		}
	}
}
