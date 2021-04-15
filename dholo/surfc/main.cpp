#include <Windows.h> 
#include <GL/gl.h> 

#include <CL/cl2.h>

#define CL_HPP_TARGET_OPENCL_VERSION 220

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

#pragma comment(lib, "OpenCL.lib")
#pragma comment(lib, "OpenGL32.lib")

using texture_t = GLuint;

HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;
float * pixels;
texture_t texture[4];

PAINTSTRUCT paint;

int WIDTH = 1200;
int HEIGHT = 800;

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL bSetupPixelFormat(HDC);

cl_platform_id * platform;
cl_device_id * device;
cl_context context;
cl_kernel kernel;
cl_command_queue command_queue;
cl_program program;
cl_mem mem;
cl_mem mem1;
cl_mem mem2;
cl_mem mem3;
cl_int err;

void InitOpenCL();
void Render();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG        msg;
	WNDCLASSW   wndclass;
	ZeroMemory(&wndclass, sizeof(WNDCLASSW));
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = L"Win OpenGL";

	if (!RegisterClassW(&wndclass))
		return FALSE;

	ghWnd = CreateWindowW(L"Win OpenGL",
		L"Generic OpenGL Sample",
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIDTH,
		HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!ghWnd)
		return FALSE;

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

		InitOpenCL();
		int chn = 0;
		stbi_set_flip_vertically_on_load(true);
		pixels = stbi_loadf("1572778667_3.jpg", &WIDTH, &HEIGHT, &chn, 0);
		int d = 0;
	}
	break;

	case WM_PAINT:

		Render();

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

	pixelformat = ChoosePixelFormat(hdc, ppfd);

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
		CL_GL_CONTEXT_KHR, (cl_context_properties)ghRC,
		CL_WGL_HDC_KHR, (cl_context_properties)ghDC,
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


	kernel = clCreateKernel(program, "testKernelWrite", &err);
	if (err != CL_SUCCESS)
		return;

	command_queue = clCreateCommandQueueWithProperties(context, device[0], NULL, &err);
	if (err != CL_SUCCESS)
		return;
}

void Render()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(4, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels);

	mem = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, texture[0], &err);

	if (err != CL_SUCCESS)
		return;

	mem1 = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, texture[1], &err);

	if (err != CL_SUCCESS)
		return;

	mem2 = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, texture[2], &err);

	if (err != CL_SUCCESS)
		return;

	mem3 = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, texture[3], &err);

	if (err != CL_SUCCESS)
		return;

	glFinish();

	clEnqueueAcquireGLObjects(command_queue, 1, &mem, 0, 0, NULL);
	clEnqueueAcquireGLObjects(command_queue, 1, &mem1, 0, 0, NULL);
	clEnqueueAcquireGLObjects(command_queue, 1, &mem2, 0, 0, NULL);
	clEnqueueAcquireGLObjects(command_queue, 1, &mem3, 0, 0, NULL);

	cl_int res = clSetKernelArg(kernel, 0, sizeof(mem), &mem);
	res = clSetKernelArg(kernel, 1, sizeof(mem1), &mem1);
	res = clSetKernelArg(kernel, 2, sizeof(mem1), &mem2);
	res = clSetKernelArg(kernel, 3, sizeof(mem1), &mem3);

	const std::size_t global_size[2] = { WIDTH, HEIGHT };
	const std::size_t local_size[2] = { 1,1 };
	clEnqueueNDRangeKernel(command_queue, kernel, 2, 0, global_size, local_size, 0, NULL, NULL);
	clEnqueueReleaseGLObjects(command_queue, 1, &mem, 0, 0, NULL);
	err = clFinish(command_queue);

	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-0.9f, 0.9f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(-0.1f, 0.9f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(-0.1f, 0.1f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-0.9f, 0.1f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.1f, 0.9f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(0.9f, 0.9f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(0.9f, 0.1f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.1f, 0.1f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-0.9f, -0.1f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(-0.1f, -0.1f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(-0.1f, -0.9f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-0.9f, -0.9f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.1f, -0.1f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(0.9f, -0.1f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(0.9f, -0.9f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.1f, -0.9f);

	glEnd();
	
	SwapBuffers(ghDC);
}