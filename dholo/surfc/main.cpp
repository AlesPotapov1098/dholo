#include <Windows.h> 
#include <GL/gl.h> 

#include <CL/cl.h>


#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

#pragma comment(lib, "OpenCL.lib")
#pragma comment(lib, "OpenGL32.lib")

# define PI           3.14159265358979323846

using texture_t = GLuint;

HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;

texture_t texture[5];
texture_t Tex;

PAINTSTRUCT paint;

const int WIDTH = 1024;
const int HEIGHT = 1024;

const int W = 1000;
const int H = 1000;

float pixels[5][WIDTH * HEIGHT * 3];
float sinus[4][WIDTH];
float fft_sin[WIDTH];

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL bSetupPixelFormat(HDC);

cl_platform_id * platform;
cl_device_id * device;
cl_context context;
cl_kernel kernel;
cl_command_queue command_queue;
cl_program program;
cl_mem mem[5];
cl_mem mem1[3];
cl_int err;

void Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

void InitOpenCL();
void Render();
void GenSinus();

void ortho(float * res, float * vec);
void mul(float * res, float * a, float * b);
void fft();

int bit_reversed(int x, int bits) {
	int y = 0;
#pragma unroll 
	for (int i = 0; i < bits; i++) {
		y <<= 1;
		y |= x & 1;
		x >>= 1;
	}
	y &= ((1 << bits) - 1);
	return y;
}


int main()
{
	int local[4][8];
	int global[16];

	for (int i = 0; i < 16; i++)
	{
		global[i] = i;
	}

	for (int i = 0; i < 4; i++)
	{
		int start_index = i * 4;
		for (int j = 0; j < 4; j++)
		{
			int index = bit_reversed(j + start_index, 4);
			local[i][j] = global[index];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int ii = 0; ii < 4; ii++)
		{
			std::cout << local[i][ii] << " ";
		}

		std::cout << std::endl;
	}

	int t = 0;
	std::cin >> t;

	return 0;
}

int main2()
{
	std::cout << "Hello OpenCL" << std::endl;

	cl_uint size1;
	err = clGetPlatformIDs(0, nullptr, &size1);

	platform = new cl_platform_id[size1];
	clGetPlatformIDs(size1, platform, nullptr);

	clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, 0, nullptr, &size1);

	device = new cl_device_id[size1];

	cl_int err = clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, size1, device, nullptr);

	if (err != CL_SUCCESS)
		return -1;

	cl_context context = clCreateContext(NULL, 1, device, NULL, NULL, &err);

	if (err != CL_SUCCESS)
		return -1;

	cl_program program;
	std::ifstream file("test.cl", std::ios_base::binary);
	std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));


	const char* src = code.c_str();
	std::size_t len = code.length() + 1;

	program = clCreateProgramWithSource(context, 1, &src, &len, &err);

	if (err != CL_SUCCESS)
		return -1;

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		std::size_t size_log = 0;
		char* build_log;
		err = clGetProgramBuildInfo(program, *device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size_log);
		if (err != CL_SUCCESS) {
			printf("Error (code) - %d\n", err);
			return err;
		}

		if (size_log == 0) {
			printf("Error (mess) - not such size_log\n");
			return -1;
		}

		build_log = new char[size_log];
		err = clGetProgramBuildInfo(program, *device, CL_PROGRAM_BUILD_LOG, size_log, build_log, nullptr);
		if (err != CL_SUCCESS) {
			printf("Error (code) - %d\n", err);
			return err;
		}
		printf("%s\n", build_log);
		delete[] build_log;
		return -1;
	}

	cl_kernel kernel = clCreateKernel(program, "testKernel", &err);
	if (err != CL_SUCCESS)
		return -1;

	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, *device, 0, &err);
	if (err != CL_SUCCESS)
		return -1;
	const cl_int m = 30;

	float input[m];

	for (int i = 0; i < m; i++)
		input[i] = i + 1;

	float output[30/5] = { 0 };

	cl_mem mem1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(input) * m, input, &err);
	if (err != CL_SUCCESS)
		return -1;

	cl_mem mem2 = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(output) * m, output, &err);
	if (err != CL_SUCCESS)
		return -1;

	err = clSetKernelArg(kernel, 0, sizeof(mem1), &mem1);
	err = clSetKernelArg(kernel, 1, sizeof(mem2), &mem2);

	std::size_t work_group = 6;
	std::size_t work_item = 2;
	err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &work_group, &work_item, 0, NULL, NULL);
	if (err != CL_SUCCESS)
		return -1;

	err = clEnqueueReadBuffer(command_queue, mem2, CL_TRUE, 0, sizeof(float) * m, output, 0, NULL, NULL);
	if (err != CL_SUCCESS)
		return -1;

	for (int i = 0; i < 6; i++)
	{
		int index = i * 5;
		float res = 0.0f;
		for (int j = index; j < index + 5; j++)
			res += input[j];
		std::cout << output[i] << " " << res << std::endl;
	}

	int t = 0;
	std::cin >> t;

	return 0;
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	
//}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;

	switch (uMsg) {

	case WM_CREATE:
	{
		ghDC = GetDC(hWnd);
		if (!bSetupPixelFormat(ghDC))
			PostQuitMessage(0);

		ghRC = wglCreateContext(ghDC);
		wglMakeCurrent(ghDC, ghRC);

		//InitOpenCL();
		GenSinus();
		
		int d = 0;
	}
	break;

	case WM_PAINT:
	{
		ghDC = BeginPaint(hWnd, &paint);
		Render();
		EndPaint(hWnd, &paint);
	}
		break;

	case WM_CLOSE:
		if (ghRC)
			wglDeleteContext(ghRC);
		if (ghDC)
			ReleaseDC(hWnd, ghDC);
		ghRC = 0;
		ghDC = 0;

		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		if (ghRC)
			wglDeleteContext(ghRC);
		if (ghDC)
			ReleaseDC(hWnd, ghDC);

		PostQuitMessage(0);
		break;

	case WM_SIZE:
	case WM_SIZING:
		return 0;

	default:
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

BOOL bSetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd, *ppfd;
	int pixelformat;

	ppfd = &pfd;

	ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	ppfd->nVersion = 1;
	ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	ppfd->dwLayerMask = PFD_MAIN_PLANE;
	ppfd->iPixelType = PFD_TYPE_RGBA;
	ppfd->cColorBits = 64;
	ppfd->cDepthBits = 32;
	ppfd->cAccumBits = 0;
	ppfd->cStencilBits = 0;

	if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0)
	{
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

void Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG        msg;
	WNDCLASSW   wndclass;
	ZeroMemory(&wndclass, sizeof(WNDCLASSW));
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = L"Win OpenGL";

	if (!RegisterClassW(&wndclass))
		return;

	ghWnd = CreateWindowW(L"Win OpenGL",
		L"Generic OpenGL Sample",
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		W,
		H,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!ghWnd)
		return;

	ShowWindow(ghWnd, nCmdShow);
	UpdateWindow(ghWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_QUIT)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void InitOpenCL()
{
	cl_uint size1;
	err = clGetPlatformIDs(0, nullptr, &size1);

	platform = new cl_platform_id[size1];
	clGetPlatformIDs(size1, platform, nullptr);

	clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, 0, nullptr, &size1);

	device = new cl_device_id[size1];

	clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, size1, device, nullptr);

	cl_context_properties prop[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform[0],
	//	CL_GL_CONTEXT_KHR, (cl_context_properties)ghRC,
	//	CL_WGL_HDC_KHR, (cl_context_properties)ghDC,
		0
	};

	context = clCreateContext(prop, 1, device, nullptr, nullptr, &err);
	if (err != CL_SUCCESS)
		return;

	std::ifstream file("simple.cl", std::ios_base::binary);
	std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));

	const char* src = code.c_str();
	std::size_t len = code.length() + 1;

	program = clCreateProgramWithSource(context, 1, &src, &len, &err);
	if (err != CL_SUCCESS)
		return;

	cl_int res = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	if (res != CL_SUCCESS)
	{
		std::size_t size_log = 0;
		char* build_log;
		res = clGetProgramBuildInfo(program, device[0], CL_PROGRAM_BUILD_LOG, 0, nullptr, &size_log);
		if (res != CL_SUCCESS) {
			printf("Error (code) - %d\n", res);
			return;
		}

		if (size_log == 0) {
			printf("Error (mess) - not such size_log\n");
			return;
		}

		build_log = new char[size_log];
		res = clGetProgramBuildInfo(program, device[0], CL_PROGRAM_BUILD_LOG, size_log, build_log, nullptr);
		if (res != CL_SUCCESS) {
			printf("Error (code) - %d\n", res);
			return;
		}
	}


	kernel = clCreateKernel(program, "psi4Kernel", &err);
	if (err != CL_SUCCESS)
		return;

	command_queue = clCreateCommandQueueWithProperties(context, device[0], NULL, &err);
	if (err != CL_SUCCESS)
		return;
}

void Render()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels[0]);

	{
		//glBindTexture(GL_TEXTURE_2D, texture[1]);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels[1]);
		//
		//glBindTexture(GL_TEXTURE_2D, texture[2]);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels[2]);
		//
		//glBindTexture(GL_TEXTURE_2D, texture[3]);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels[3]);
		//
		//glBindTexture(GL_TEXTURE_2D, texture[4]);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels[4]);
		//
		//mem[0] = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, texture[0], &err);
		//
		//if (err != CL_SUCCESS)
		//	return;
		//
		//mem[1] = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, texture[1], &err);
		//
		//if (err != CL_SUCCESS)
		//	return;
		//
		//mem[2] = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, texture[2], &err);
		//
		//if (err != CL_SUCCESS)
		//	return;
		//
		//mem[3] = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, texture[3], &err);
		//
		//if (err != CL_SUCCESS)
		//	return;
		//
		//mem[4] = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, texture[4], &err);
		//
		//if (err != CL_SUCCESS)
		//	return;
		//
		//cl_float4 C = { 0.0f, -2.0f, 0.0f, 2.0f };
		//cl_float4 S = { 2.0f, 0.0f, -2.0f, 0.0f };
		//cl_float B = 4.0f;
		//
		//glFinish();
		//
		//clEnqueueAcquireGLObjects(command_queue, 5, mem, 0, 0, NULL);
		//
		//cl_int res = clSetKernelArg(kernel, 0, sizeof(mem[0]), &mem[0]);
		//res = clSetKernelArg(kernel, 1, sizeof(mem[1]), &mem[1]);
		//res = clSetKernelArg(kernel, 2, sizeof(mem[2]), &mem[2]);
		//res = clSetKernelArg(kernel, 3, sizeof(mem[3]), &mem[3]);
		//
		//res = clSetKernelArg(kernel, 4, sizeof(cl_float4), &S);
		//res = clSetKernelArg(kernel, 5, sizeof(cl_float4), &C);
		//res = clSetKernelArg(kernel, 6, sizeof(cl_float), &B);
		//
		//res = clSetKernelArg(kernel, 7, sizeof(mem[4]), &mem[4]);
		//
		//const std::size_t global_size[2] = { WIDTH, HEIGHT };
		//const std::size_t local_size[2] = { 2,2 };
		//clEnqueueNDRangeKernel(command_queue, kernel, 2, 0, global_size, local_size, 0, NULL, NULL);
		//clEnqueueReleaseGLObjects(command_queue, 5, mem, 0, 0, NULL);
		//err = clFinish(command_queue);
	}

	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, 1.0f);
	
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
	
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);
	
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, -1.0f);
	
	glEnd();
	
	SwapBuffers(ghDC);
}

void GenSinus()
{
	int N = WIDTH * HEIGHT;

	float phi1 = 0,
		phi2 = PI / 2,
		phi3 = PI,
		phi4 = 3 * PI / 2;

	int K = 10;

	int k = 0;

	for (int i = 0; i < WIDTH; i++)
	{
		sinus[0][i] = (sinf((2 * PI * K / WIDTH) * i + phi1) + 1) / 2;
		sinus[1][i] = (sinf((2 * PI * K / WIDTH) * i + phi2) + 1) / 2;
		sinus[2][i] = (sinf((2 * PI * K / WIDTH) * i + phi3) + 1) / 2;
		sinus[3][i] = (sinf((2 * PI * K / WIDTH) * i + phi4) + 1) / 2;
	}

	fft();

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			float n1 = sinus[0][j],
			  n2 = sinus[1][j],
			  n3 = sinus[2][j],
			  n4 = sinus[3][j];
			
			pixels[0][k] = n1;
			pixels[1][k] = n2;
			pixels[2][k] = n3;
			pixels[3][k] = n4;
			pixels[4][k] = 0.0f;
			
			k++;
			
			pixels[0][k] = n1;
			pixels[1][k] = n2;
			pixels[2][k] = n3;
			pixels[3][k] = n4;
			pixels[4][k] = 0.0f;
			
			k++;
			
			pixels[0][k] = n1;
			pixels[1][k] = n2;
			pixels[2][k] = n3;
			pixels[3][k] = n4;
			pixels[4][k] = 0.0f;
			
			k++;
		}
	}
}

void ortho(float * res, float * vec)
{
	if (!vec)
		return;

	res[0] = vec[0] * 0.0f + vec[1] * 1.0f + vec[2] * 0.0f + vec[3] * (-1.0f);
	res[1] = vec[0] * (-1.0f) + vec[1] * 0.0f + vec[2] * 1.0f + vec[3] * 0.0f;
	res[2] = vec[0] * 0.0f + vec[1] * (-1.0f) + vec[2] * 0.0f + vec[3] * 1.0f;
	res[3] = vec[0] * 1.0f + vec[1] * 0.0f + vec[2] * (-1.0f) + vec[3] * 0.0f;
}

void mul(float * res, float * a, float * b)
{
	*res = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

void fft()
{
	int m = 10;
	int i, j, ip, k, l;
	int n = (int)pow(2., m); 
	int n1 = n >> 1;
	
	for (i = 0, j = 0, k = n1; i<n - 1; i++, j = j + k)
	{
		if (i<j) 
		{ 
			float t = sinus[0][j]; 
			sinus[0][j] = sinus[0][i];
			sinus[0][i] = t;
		}

		k = n1;
		
		while (k <= j) 
		{ 
			j = j - k; 
			k = k >> 1; 
		}
	}

	for (l = 1; l <= m; l++)
	{
		int ll = (int)pow(2., l);
		int ll1 = ll >> 1;

		float U_Re = 1.0f;
		float U_Im = 0.0f;

		float W_Re = cos(CL_M_PI / ll1);
		float W_Im = sin(CL_M_PI / ll1);
		
		for (j = 1; j <= ll1; j++)
		{
			for (i = j - 1; i < n; i = i + ll)
			{
				ip = i + ll1; 
				float t = sinus[0][ip] * U_Re; 
				sinus[0][ip] = sinus[0][i] - t; 
				sinus[0][i] = sinus[0][i] + t;
			}

			U_Re = U_Re * W_Re - U_Im * W_Im;
			U_Im = U_Re * W_Im + U_Im * W_Re;
		}
	}
}
