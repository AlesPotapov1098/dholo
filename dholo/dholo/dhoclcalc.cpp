#include "stdafx.h"
#include "dhoclcalc.h"

void DHOCLCalc::Init(const DHOCLHost& host, const std::string& progpath)
{
	if (progpath.empty())
		return;

	cl_platform_id pl;
	clGetPlatformIDs(1, &pl, NULL);

	cl_device_id dev;
	clGetDeviceIDs(pl, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);

	cl_int err;
	cl_context_properties prop[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties) pl,
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		0};


	m_Context = clCreateContext(prop, 1, &dev, NULL, NULL, &err);

	if (err != CL_SUCCESS)
		return;

	std::ifstream file(progpath, std::ios_base::binary);
	std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));


	const char * src = code.c_str();
	std::size_t len = code.length() + 1;

	m_Program = clCreateProgramWithSource(m_Context, 1, &src, &len, &err);

	if (err != CL_SUCCESS)
		return;

	cl_int res = clBuildProgram(m_Program, 0, NULL, NULL, NULL, NULL);

	if (res != CL_SUCCESS)
	{
		std::size_t size_log = 0;
		char* build_log;
		res = clGetProgramBuildInfo(m_Program, dev, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size_log);
		if (res != CL_SUCCESS) {
			printf("Error (code) - %d\n", res);
			return;
		}

		if (size_log == 0) {
			printf("Error (mess) - not such size_log\n");
			return;
		}

		build_log = new char[size_log];
		res = clGetProgramBuildInfo(m_Program, dev, CL_PROGRAM_BUILD_LOG, size_log, build_log, nullptr);
		if (res != CL_SUCCESS) {
			printf("Error (code) - %d\n", res);
			return;
		}
	}

	m_Kernel = clCreateKernel(m_Program, "testKernelWrite", &err);
	if (err != CL_SUCCESS)
		return;

	m_CommandQueue = clCreateCommandQueueWithProperties(m_Context, dev, NULL, &err);
	if (err != CL_SUCCESS)
		return;
	
}

void DHOCLCalc::LoadImg(const GLuint& texture)
{
	cl_int error_code = CL_SUCCESS;
	m_InOutMem = clCreateFromGLTexture(m_Context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, texture, &error_code);

	if (error_code != CL_SUCCESS)
		return;
}

void DHOCLCalc::SetGlobalPar(int global_x = 0, int global_y = 0)
{
	m_GlobalX = global_x;
	m_GlobalY = global_y;
}

void DHOCLCalc::SetLocalPar(int local_x = 1, int local_y = 1)
{
	m_LocalX = local_x;
	m_LocalY = local_y;
}

void DHOCLCalc::Calculate()
{
	clEnqueueAcquireGLObjects(m_CommandQueue, 1, &m_InOutMem, 0, 0, NULL);
	cl_int res = clSetKernelArg(m_Kernel, 0, sizeof(m_InOutMem), &m_InOutMem);

	if (m_GlobalX <= 0 || m_GlobalY <= 0)
		return;

	const std::size_t global_size[2] = { m_GlobalX, m_GlobalY };
	const std::size_t local_size[2] = { m_LocalX, m_LocalY };

	clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, 0, global_size, local_size, 0, NULL, NULL);

	clEnqueueReleaseGLObjects(m_CommandQueue, 1, &m_InOutMem, 0, 0, NULL);
	cl_int err = clFinish(m_CommandQueue);
}
