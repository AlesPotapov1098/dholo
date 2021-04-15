#include "gpgpu.h"



GPGPU::GPGPU()
{
	m_TexWidth = 0;
	m_TexHeight = 0;
	ZeroMemory(&m_PixelFormat, sizeof(PIXELFORMATDESCRIPTOR));
}

GPGPU::~GPGPU()
{
	m_TexWidth = 0;
	m_TexHeight = 0;


	ReleaseOpenGL();
	ReleaseOpenCL();
}

void GPGPU::InitOpenGL(HDC hDC)
{
	m_hDC = hDC;

	m_PixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	m_PixelFormat.nVersion = 1;
	m_PixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	m_PixelFormat.dwLayerMask = PFD_MAIN_PLANE;
	m_PixelFormat.iPixelType = PFD_TYPE_RGBA;
	m_PixelFormat.cColorBits = 64;
	m_PixelFormat.cDepthBits = 32;
	m_PixelFormat.cAccumBits = 0;
	m_PixelFormat.cStencilBits = 0;

	int pxlfrom = ChoosePixelFormat(m_hDC, &m_PixelFormat);
	if (pxlfrom == 0)
		return;

	if (!SetPixelFormat(m_hDC, pxlfrom, &m_PixelFormat))
		return;
}

void GPGPU::InitOpenCL()
{
	cl_uint size1;
	m_ErrorCode = clGetPlatformIDs(0, nullptr, &size1);

	m_Platform = new cl_platform_id[size1];
	clGetPlatformIDs(size1, m_Platform, nullptr);

	clGetDeviceIDs(m_Platform[0], CL_DEVICE_TYPE_GPU, 0, nullptr, &size1);

	m_Device = new cl_device_id[size1];

	clGetDeviceIDs(m_Platform[0], CL_DEVICE_TYPE_GPU, size1, m_Device, nullptr);

	cl_context_properties prop[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties)m_Platform[0],
		CL_GL_CONTEXT_KHR, (cl_context_properties)m_hRC,
		CL_WGL_HDC_KHR, (cl_context_properties)m_hDC,
		0
	};

	m_Context = clCreateContext(prop, 1, m_Device, nullptr, nullptr, &m_ErrorCode);
	if (m_ErrorCode != CL_SUCCESS)
		return;

	std::ifstream file("simple.cl", std::ios_base::binary);
	std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));

	const char* src = code.c_str();
	std::size_t len = code.length() + 1;

	m_Program = clCreateProgramWithSource(m_Context, 1, &src, &len, &m_ErrorCode);
	if (m_ErrorCode != CL_SUCCESS)
		return;

	cl_int res = clBuildProgram(m_Program, 0, NULL, NULL, NULL, NULL);

	if (res != CL_SUCCESS)
	{
		std::size_t size_log = 0;
		char* build_log;
		res = clGetProgramBuildInfo(m_Program, m_Device[0], CL_PROGRAM_BUILD_LOG, 0, nullptr, &size_log);
		if (res != CL_SUCCESS) {
			printf("Error (code) - %d\n", res);
			return;
		}

		if (size_log == 0) {
			printf("Error (mess) - not such size_log\n");
			return;
		}

		build_log = new char[size_log];
		res = clGetProgramBuildInfo(m_Program, m_Device[0], CL_PROGRAM_BUILD_LOG, size_log, build_log, nullptr);
		if (res != CL_SUCCESS) {
			printf("Error (code) - %d\n", res);
			return;
		}
	}

	m_Kernel = clCreateKernel(m_Program, "testKernelWrite", &m_ErrorCode);
	if (m_ErrorCode != CL_SUCCESS)
		return;

	m_CommandQueue = clCreateCommandQueueWithProperties(m_Context, m_Device[0], NULL, &m_ErrorCode);
	if (m_ErrorCode != CL_SUCCESS)
		return;
}

void GPGPU::UpdateOpenGL(HDC hDC)
{
}

void GPGPU::Execute()
{
}

void GPGPU::DrawScene()
{
}

void GPGPU::ReleaseOpenGL()
{
	ZeroMemory(&m_PixelFormat, sizeof(PIXELFORMATDESCRIPTOR));
	if(m_Texture)
		delete [] m_Texture;
}

void GPGPU::ReleaseOpenCL()
{
}
