#include "stdafx.h"
#include "dhoclcalc.h"

DHOCLCalc::DHOCLCalc()
{
	m_Program = NULL;
	m_Kernel = NULL;
	m_Context = NULL;
	m_InOutMem = NULL;
	m_CommandQueue = NULL;
}

DHOCLCalc::~DHOCLCalc()
{
	Release();
}

void DHOCLCalc::Init(const DHOCLHost& host, const std::string& progpath)
{
	if (progpath.empty())
		return;

	cl_int err;
	cl_context_properties prop[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties) host.GetPlatform(),
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		0};

	cl_device_id dev = host.GetDevice();
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

void DHOCLCalc::SetGlobalSize(std::size_t global_x = 0, std::size_t global_y = 0)
{
	m_GlobalSize[0] = global_x;
	m_GlobalSize[1] = global_y;
}

void DHOCLCalc::SetGlobalSize(std::size_t globalSize[2])
{
	m_GlobalSize[0] = globalSize[0];
	m_GlobalSize[1] = globalSize[1];
}

void DHOCLCalc::SetLocalSize(std::size_t local_x = 1, std::size_t local_y = 1)
{
	m_LocalSize[0] = local_x;
	m_LocalSize[1] = local_y;
}

void DHOCLCalc::SetLocalSize(std::size_t localSize[2])
{
	m_LocalSize[0] = localSize[0];
	m_LocalSize[1] = localSize[1];
}

void DHOCLCalc::Calculate()
{
	clEnqueueAcquireGLObjects(m_CommandQueue, 1, &m_InOutMem, 0, 0, NULL);
	cl_int res = clSetKernelArg(m_Kernel, 0, sizeof(m_InOutMem), &m_InOutMem);

	if (m_GlobalSize[0] <= 0 || m_GlobalSize[1] <= 0)
		return;

	clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, 0, m_GlobalSize, m_LocalSize, 0, NULL, NULL);

	clEnqueueReleaseGLObjects(m_CommandQueue, 1, &m_InOutMem, 0, 0, NULL);
	cl_int err = clFinish(m_CommandQueue);
}

void DHOCLCalc::Release()
{
	if(m_Program)
		clReleaseProgram(m_Program);
	
	if(m_Kernel)
		clReleaseKernel(m_Kernel);
	
	if(m_Context)
		clReleaseContext(m_Context);
	
	if(m_CommandQueue)
		clReleaseCommandQueue(m_CommandQueue);
	
	if(m_InOutMem)
		clReleaseMemObject(m_InOutMem);
}
