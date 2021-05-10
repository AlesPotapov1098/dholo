#include "pch.h"
#include "DHOCLTransform.h"

namespace dholo
{
	namespace gpgpu
	{
		//DHOCLTransform::DHOCLTransform()
		//{
		//	m_Program = NULL;
		//	m_Kernel = NULL;
		//	m_Context = NULL;
		//	m_InOutMem = NULL;
		//	m_CommandQueue = NULL;
		//}
		//
		//DHOCLTransform::DHOCLTransform(const DHOCLHost& host, const std::string& progpath)
		//{
		//	if (progpath.empty())
		//		return;
		//
		//	cl_int err;
		//	cl_context_properties prop[] = {
		//		CL_CONTEXT_PLATFORM, (cl_context_properties)host.GetPlatform(),
		//		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		//		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		//		0 };
		//
		//	cl_device_id dev = host.GetDevice();
		//	m_Context = clCreateContext(prop, 1, &dev, NULL, NULL, &err);
		//
		//	if (err != CL_SUCCESS)
		//		return;
		//
		//	std::ifstream file(progpath, std::ios_base::binary);
		//	std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
		//
		//
		//	const char* src = code.c_str();
		//	std::size_t len = code.length() + 1;
		//
		//	m_Program = clCreateProgramWithSource(m_Context, 1, &src, &len, &err);
		//
		//	if (err != CL_SUCCESS)
		//		return;
		//
		//	cl_int res = clBuildProgram(m_Program, 0, NULL, NULL, NULL, NULL);
		//
		//	if (res != CL_SUCCESS)
		//	{
		//		std::size_t size_log = 0;
		//		char* build_log;
		//		res = clGetProgramBuildInfo(m_Program, dev, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size_log);
		//		if (res != CL_SUCCESS) {
		//			printf("Error (code) - %d\n", res);
		//			return;
		//		}
		//
		//		if (size_log == 0) {
		//			printf("Error (mess) - not such size_log\n");
		//			return;
		//		}
		//
		//		build_log = new char[size_log];
		//		res = clGetProgramBuildInfo(m_Program, dev, CL_PROGRAM_BUILD_LOG, size_log, build_log, nullptr);
		//		if (res != CL_SUCCESS) {
		//			printf("Error (code) - %d\n", res);
		//			return;
		//		}
		//	}
		//
		//	m_Kernel = clCreateKernel(m_Program, "psi4Kernel", &err);
		//	if (err != CL_SUCCESS)
		//		return;
		//
		//	m_CommandQueue = clCreateCommandQueueWithProperties(m_Context, dev, NULL, &err);
		//	if (err != CL_SUCCESS)
		//		return;
		//}
		//
		//DHOCLTransform::~DHOCLTransform()
		//{
		//	if (m_Program)
		//		clReleaseProgram(m_Program);
		//
		//	if (m_Kernel)
		//		clReleaseKernel(m_Kernel);
		//
		//	if (m_Context)
		//		clReleaseContext(m_Context);
		//
		//	if (m_CommandQueue)
		//		clReleaseCommandQueue(m_CommandQueue);
		//
		//	if (m_InOutMem)
		//		clReleaseMemObject(m_InOutMem);
		//}
		//
		//void DHOCLTransform::LoadImg(const GLuint& texture)
		//{
		//	cl_int error_code = CL_SUCCESS;
		//	m_InOutMem = clCreateFromGLTexture(m_Context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, texture, &error_code);
		//
		//	if (error_code != CL_SUCCESS)
		//		return;
		//}
		//
		//void DHOCLTransform::SetGlobalSize(std::size_t global_x = 0, std::size_t global_y = 0)
		//{
		//	m_GlobalSize[0] = global_x;
		//	m_GlobalSize[1] = global_y;
		//}
		//
		//void DHOCLTransform::SetGlobalSize(std::size_t globalSize[2])
		//{
		//	m_GlobalSize[0] = globalSize[0];
		//	m_GlobalSize[1] = globalSize[1];
		//}
		//
		//void DHOCLTransform::SetLocalSize(std::size_t local_x = 1, std::size_t local_y = 1)
		//{
		//	m_LocalSize[0] = local_x;
		//	m_LocalSize[1] = local_y;
		//}
		//
		//void DHOCLTransform::SetLocalSize(std::size_t localSize[2])
		//{
		//	m_LocalSize[0] = localSize[0];
		//	m_LocalSize[1] = localSize[1];
		//}
		//
		//void DHOCLTransform::Calculate()
		//{
		//	clEnqueueAcquireGLObjects(m_CommandQueue, 1, &m_InOutMem, 0, 0, NULL);
		//	cl_int res = clSetKernelArg(m_Kernel, 0, sizeof(m_InOutMem), &m_InOutMem);
		//
		//	if (m_GlobalSize[0] <= 0 || m_GlobalSize[1] <= 0)
		//		return;
		//
		//	clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, 0, m_GlobalSize, m_LocalSize, 0, NULL, NULL);
		//
		//	clEnqueueReleaseGLObjects(m_CommandQueue, 1, &m_InOutMem, 0, 0, NULL);
		//	cl_int err = clFinish(m_CommandQueue);
		//}

		DHGPGPUTransform::DHGPGPUTransform()
		{
			m_Program = nullptr;
			m_Kernel = nullptr;
			m_InOutMem = nullptr;
			m_Context = nullptr;
			m_CommandQueue = nullptr;

			m_hRC = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));

			m_Desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			m_Desc.nVersion = 1;
			m_Desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			m_Desc.iPixelType = PFD_TYPE_RGBA;
			m_Desc.cColorBits = 32;
			m_Desc.cDepthBits = 24;
			m_Desc.iLayerType = PFD_MAIN_PLANE;
		}

		DHGPGPUTransform::~DHGPGPUTransform()
		{
			Release();
		}

		void DHGPGPUTransform::Init(const CDC& dc, const DHOCLHost& host)
		{
			if (dc == nullptr)
				return;

			m_hDC = dc.m_hDC;

			if (!SetPixelFormat(m_hDC,
				ChoosePixelFormat(m_hDC, &m_Desc),
				&m_Desc))
				return;

			m_hRC = wglCreateContext(m_hDC);
			if (!m_hRC)
				return;

			if (!wglMakeCurrent(m_hDC, m_hRC))
				return;

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}
		
		void DHGPGPUTransform::GenerateTexture() { return; }

		void DHGPGPUTransform::Calculate() { return; }

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

		void DHGPGPUTransform::RenderScene()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			SwapBuffers(m_hDC);
		}

		void DHGPGPUTransform::InitOpenCL(const DHOCLHost& host)
		{
			cl_int err = CL_SUCCESS;
			cl_context_properties prop[] = {
				CL_CONTEXT_PLATFORM, (cl_context_properties)host.GetPlatform(),
				CL_GL_CONTEXT_KHR, (cl_context_properties)m_hRC,
				CL_WGL_HDC_KHR, (cl_context_properties)m_hDC,
				0 };
			
			cl_device_id dev = host.GetDevice();
			m_Context = clCreateContext(prop, 1, &dev, NULL, NULL, &err);
			
			if (err != CL_SUCCESS)
				return;
			
			std::ifstream file(m_ProgramPath, std::ios_base::binary);
			std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
			
			const char* src = code.c_str();
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
					/// TODO : обработка ошибок
					return;
				}

				if (size_log == 0) {
					/// TODO : обработка ошибок
					return;
				}

				build_log = new char[size_log];
				res = clGetProgramBuildInfo(m_Program, dev, CL_PROGRAM_BUILD_LOG, size_log, build_log, nullptr);
				if (res != CL_SUCCESS) {
					/// TODO : обработка ошибок
					return;
				}
			}

			m_Kernel = clCreateKernel(m_Program, "psi4Kernel", &err);
			if (err != CL_SUCCESS)
				/// TODO : обработка ошибок
				return;
			
			m_CommandQueue = clCreateCommandQueueWithProperties(m_Context, dev, NULL, &err);
			if (err != CL_SUCCESS)
				/// TODO : обработка ошибок
				return;
		}

		DHGPGPUPSITransform::DHGPGPUPSITransform()
		{
			DHGPGPUTransform::DHGPGPUTransform();
		}

		DHGPGPUPSITransform::~DHGPGPUPSITransform()
		{
			DHGPGPUTransform::~DHGPGPUTransform();
		}

		void DHGPGPUPSITransform::Init(const CDC& dc, const DHOCLHost& host)
		{
			DHGPGPUTransform::Init(dc, host);
			InitOpenCL(host);
		}

		void DHGPGPUPSITransform::SetImages(const std::vector<dholo::img::DHImgLoader>& imgs)
		{
			m_Images = imgs;
		}

		void DHGPGPUPSITransform::GenerateTexture()
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(5, m_Textures);

			cl_int error_code = CL_SUCCESS;

			for (int i = 0; i < 5; i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_Textures[i]);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if (m_Images[i].GetChannels() == 3)
					glTexImage2D(
						GL_TEXTURE_2D, 0, GL_RGB,
						m_Images[i].GetWidth(),
						m_Images[i].GetHeight(),
						0, GL_RGB, GL_FLOAT,
						m_Images[i].GetPixelsData());
				else if (m_Images[i].GetChannels() == 4)
					glTexImage2D(
						GL_TEXTURE_2D, 0, GL_RGBA,
						m_Images[i].GetWidth(),
						m_Images[i].GetHeight(),
						0, GL_RGBA, GL_FLOAT,
						m_Images[i].GetPixelsData());

				m_Mem[i] = clCreateFromGLTexture(
					m_Context, 
					CL_MEM_READ_ONLY, 
					GL_TEXTURE_2D, 
					0, m_Textures[i], 
					&error_code);

				if (error_code != CL_SUCCESS)
					/// TODO: обработка ошибок
					return;
			}
		}

		void DHGPGPUPSITransform::Calculate()
		{
		}

		void DHGPGPUPSITransform::Release()
		{
		}

		void DHGPGPUPSITransform::RenderScene()
		{
		}

		DHGPGPUGenSinus::DHGPGPUGenSinus(int width, int height, float ampl, float phi, int T)
		{
			DHGPGPUTransform::DHGPGPUTransform();

			m_width = width;
			m_height = height;
			m_amp = ampl;
			m_phase = phi;
			m_T = T;
		}

		DHGPGPUGenSinus::~DHGPGPUGenSinus()
		{
			DHGPGPUTransform::~DHGPGPUTransform();
		}

		void DHGPGPUGenSinus::GenerateTexture()
		{
			float *sinus = new float[m_height];
			float coeff = 2 * CL_M_PI * m_T / m_height;
			for (int n = 0; n < m_height; n++)
				sinus[n] = m_amp * ((std::sinf(coeff * n + m_phase) + 1) / 2);

			float* pixels = new float[m_width * m_height * 3];
			int k = 0;
			for(int i = 0; i < m_width; i++)
				for (int j = 0; j < m_height; j++)
				{
					pixels[k++] = sinus[j];
					pixels[k++] = sinus[j];
					pixels[k++] = sinus[j];
				}

			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &m_SinTexture);
			glBindTexture(GL_TEXTURE_2D, m_SinTexture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_FLOAT, pixels);

			delete[] pixels;
			delete[] sinus;
		}

		void DHGPGPUGenSinus::Calculate()
		{
		}

		void DHGPGPUGenSinus::Release()
		{
			//DHGPGPUTransform::~DHGPGPUTransform();
		}

		void DHGPGPUGenSinus::RenderScene()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,m_SinTexture);

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
	}
}