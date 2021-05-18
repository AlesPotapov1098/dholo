#include "pch.h"
#include "DHOCLTransform.h"

namespace dholo
{
	namespace gpgpu
	{
		DHGPGPUTransform::DHGPGPUTransform()
		{
			m_Program = nullptr;
			m_Kernel = nullptr;
			m_InOutMem = nullptr;
			m_Context = nullptr;
			m_CommandQueue = nullptr;

			m_KernelName = "";
			m_ProgramPath = "";
		}

		DHGPGPUTransform::~DHGPGPUTransform()
		{
			Release();
		}

		void DHGPGPUTransform::Init(const DHOCLHost& host, HDC dc, HGLRC rc)
		{
			if(m_KernelName.empty() || m_ProgramPath.empty())
				return;

			cl_int err = CL_SUCCESS;
			cl_context_properties prop[] = {
				CL_CONTEXT_PLATFORM, (cl_context_properties)host.GetPlatform(),
				CL_GL_CONTEXT_KHR, (cl_context_properties)rc,
				CL_WGL_HDC_KHR, (cl_context_properties)dc,
				0 };

			cl_device_id dev = host.GetDevice();
			m_Context = clCreateContext(prop, 1, &dev, NULL, NULL, &err);

			if (err != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(err);

			std::ifstream file(m_ProgramPath, std::ios_base::binary);
			std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));

			const char* src = code.c_str();
			std::size_t len = code.length() + 1;

			m_Program = clCreateProgramWithSource(m_Context, 1, &src, &len, &err);

			if (err != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(err);

			err = clBuildProgram(m_Program, 1, &dev, NULL, NULL, NULL);

			if (err != CL_SUCCESS)
			{
				cl_int code = err;
				std::size_t size_log = 0;
				char* build_log;
				err = clGetProgramBuildInfo(m_Program, dev, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size_log);
				if (err != CL_SUCCESS)
					throw dholo::exp::DHGPGPUExp(err);

				if (size_log == 0)
					throw dholo::exp::DHAppExp("Can't load compiler log");

				build_log = new char[size_log];
				err = clGetProgramBuildInfo(m_Program, dev, CL_PROGRAM_BUILD_LOG, size_log, build_log, nullptr);
				if (err == CL_SUCCESS)
					throw dholo::exp::DHGPGPUExp(code, build_log);
				else
					throw dholo::exp::DHGPGPUExp(err);
			}

			m_Kernel = clCreateKernel(m_Program, m_KernelName.c_str(), &err);
			if (err != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(err);

			m_CommandQueue = clCreateCommandQueueWithProperties(m_Context, dev, NULL, &err);
			if (err != CL_SUCCESS)
				throw dholo::exp::DHGPGPUExp(err);
		}

		void DHGPGPUTransform::Calculate(int global_w, int global_h, int local_w, int local_h) 
		{
			if (m_KernelName.empty() || m_ProgramPath.empty())
				return;
		}

		void DHGPGPUTransform::Release()
		{
			if (m_Program)
				clReleaseProgram(m_Program);

			if (m_Kernel)
				clReleaseKernel(m_Kernel);

			if (m_Context)
				clReleaseContext(m_Context);

			if (m_CommandQueue)
				clReleaseCommandQueue(m_CommandQueue);

			if (m_InOutMem)
				clReleaseMemObject(m_InOutMem);
		}

		void DHGPGPUTransform::SetProgramName(const std::string& prgpath)
		{
			m_ProgramPath = prgpath;
		}

		void DHGPGPUTransform::SetKernelName(const std::string& krnlname)
		{
			m_KernelName = krnlname;
		}
	}
}