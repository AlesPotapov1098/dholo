#include "stdafx.h"

#include "dhocltest.h"

namespace dholo
{
	namespace test
	{
		DHOCLTest::DHOCLTest()
		{
		}

		DHOCLTest::DHOCLTest(const ocl::DHOCLHost & host, const std::string & progpath)
		{
			DHOCLCalc(host, progpath);
		}

		DHOCLTest::~DHOCLTest()
		{
		}

		void DHOCLTest::LoadImg(const ogl::DHOGLRender & scene)
		{
			cl_int error_code = CL_SUCCESS;
			m_Mem[0] = clCreateFromGLTexture(m_Context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, scene.GetTexture(0), &error_code);

			if (error_code != CL_SUCCESS)
				return;

			m_Mem[1] = clCreateFromGLTexture(m_Context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, scene.GetTexture(1), &error_code);

			if (error_code != CL_SUCCESS)
				return;

			m_Mem[2] = clCreateFromGLTexture(m_Context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, scene.GetTexture(2), &error_code);

			if (error_code != CL_SUCCESS)
				return;

			m_Mem[3] = clCreateFromGLTexture(m_Context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, scene.GetTexture(3), &error_code);

			if (error_code != CL_SUCCESS)
				return;

			m_Mem[4] = clCreateFromGLTexture(m_Context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, scene.GetTexture(4), &error_code);

			if (error_code != CL_SUCCESS)
				return;
		}

		void DHOCLTest::Calculate()
		{
			clEnqueueAcquireGLObjects(m_CommandQueue, 5, m_Mem, 0, 0, NULL);
			cl_int res = clSetKernelArg(m_Kernel, 0, sizeof(m_Mem[0]), &m_Mem[0]);
			cl_int res = clSetKernelArg(m_Kernel, 1, sizeof(m_Mem[1]), &m_Mem[1]);
			cl_int res = clSetKernelArg(m_Kernel, 2, sizeof(m_Mem[2]), &m_Mem[2]);
			cl_int res = clSetKernelArg(m_Kernel, 3, sizeof(m_Mem[3]), &m_Mem[3]);

			cl_float4 C = { 0.0f, -2.0f, 0.0f, 2.0f };
			cl_float4 S = { 2.0f, 0.0f, -2.0f, 0.0f };
			cl_float B = 4.0f;

			res = clSetKernelArg(m_Kernel, 4, sizeof(cl_float4), &S);
			res = clSetKernelArg(m_Kernel, 5, sizeof(cl_float4), &C);
			res = clSetKernelArg(m_Kernel, 6, sizeof(cl_float), &B);

			cl_int res = clSetKernelArg(m_Kernel, 7, sizeof(m_Mem[4]), &m_Mem[4]);

			if (m_GlobalSize[0] <= 0 || m_GlobalSize[1] <= 0)
				return;

			clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, 0, m_GlobalSize, m_LocalSize, 0, NULL, NULL);

			clEnqueueReleaseGLObjects(m_CommandQueue, 5, m_Mem, 0, 0, NULL);
			cl_int err = clFinish(m_CommandQueue);
		}
	}
}
