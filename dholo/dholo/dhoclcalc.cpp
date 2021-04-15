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
	cl_context_properties properties[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties) pl,
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		0};

	cl::Device device(dev);

	m_Context = cl::Context(device, properties, NULL, NULL, &err);

	std::ifstream file(progpath, std::ios_base::binary);
	std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
	cl::Program::Sources src(1, cl::string(code.c_str()));

	m_Program = cl::Program(m_Context, src);
	err = m_Program.build(device);
	if (err != CL_SUCCESS)
	{
		auto msg = m_Program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device, &err);
		return;
	}

	m_Kernel = cl::Kernel(m_Program, "testKernelWrite", &err);

	m_CommandQueue = cl::CommandQueue(m_Context, device, CL_QUEUE_PROFILING_ENABLE, &err);
	int d = 0;
}

void DHOCLCalc::LoadImg(const GLuint& texture, int width, int height)
{
	cl_int error_code = 847489;
	m_width = width;
	m_height = height;
	m_InOutMem = cl::ImageGL(m_Context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture, &error_code);
	int d = 0;
}

void DHOCLCalc::Calculate()
{
	cl::Event ev;
	cl::vector<cl::Memory> vec;
	vec.push_back(m_InOutMem);
	cl_int err = m_CommandQueue.enqueueAcquireGLObjects(&vec, NULL, &ev);
	err = ev.wait();
	err = m_Kernel.setArg(0, m_InOutMem);
	cl::NDRange offset = 0;
	cl::NDRange gloabl_size = cl::NDRange((cl::size_type)m_width, (cl::size_type)m_height);
	cl::NDRange local_size = { 1, 1 };
	err = m_CommandQueue.enqueueNDRangeKernel(m_Kernel, offset, gloabl_size, local_size, NULL, &ev);
	err = ev.wait();
	err = m_CommandQueue.enqueueReleaseGLObjects(&vec, NULL, &ev);
	err = ev.wait();
	//if (err != CL_SUCCESS)
	//{
	//	auto msg = ev.getProfilingInfo<CL_PROFILING_COMMAND_QUEUED>(&err);
	//	int d = 0;
	//}

	err = m_CommandQueue.finish();
}
