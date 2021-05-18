#include "pch.h"
#include "DHGPGPUGetPrmtrs.h"

namespace dholo
{
	namespace gpgpu
	{
		DHGPGPUGetPrmtrs::DHGPGPUGetPrmtrs(GLuint* tex)
		{
			m_Tex = tex;
			m_Mem = nullptr;
		}

		void DHGPGPUGetPrmtrs::Init(const DHOCLHost& host, HDC dc, HGLRC rc)
		{
			DHGPGPUTransform::Init(host,dc, rc);

			cl_int error_code = CL_SUCCESS;

			m_Mem = clCreateFromGLTexture(
				m_Context,
				CL_MEM_READ_WRITE,
				GL_TEXTURE_2D,
				0, *m_Tex,
				&error_code);

			if (error_code != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error_code);
		}

		void DHGPGPUGetPrmtrs::Calculate(int global_w, int global_h, int local_w, int local_h)
		{
			DHGPGPUTransform::Calculate(0, 0, 0, 0);

			cl_int error = clEnqueueAcquireGLObjects(m_CommandQueue, 1, &m_Mem, 0, 0, NULL);
			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			error |= clSetKernelArg(m_Kernel, 0, sizeof(m_Mem), &m_Mem);

			if (error != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(error);

			const std::size_t global_size[2] = {global_w, global_h };
			const std::size_t local_size[2] = { local_w, local_h };

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

		void DHGPGPUGetPrmtrs::Release()
		{
			if (m_Mem)
				clReleaseMemObject(m_Mem);

			DHGPGPUTransform::Release();
		}
	}
}
