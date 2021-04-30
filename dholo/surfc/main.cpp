#include <Windows.h> 
#include <GL/gl.h> 
#include <CL/cl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

#pragma comment(lib, "OpenCL.lib")
#pragma comment(lib, "OpenGL32.lib")

using texture_t = GLuint;

HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;

texture_t texture;

PAINTSTRUCT paint;

const int WIDTH = 1024;
const int HEIGHT = 1024;

const int W = 1000;
const int H = 1000;

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL bSetupPixelFormat(HDC);

// Генерируем текстуру OpenGL
	void gen_texture(
		// 0 -Высота
			int WIDTH,
		// 1 - ширина
			int HEIGHT,
		// 2 - Текструа
			texture_t * tex);

// Генерация синусоидального сигнала
	cl_float2 * generate_sinus(
		// Количество точек
			int N,
		// Амплитуда
			int ampl,
		// Фаза
			int phase,
		// Количество периодов
			int k);

// Инициализируем графическое устройство
	cl_device_id get_device();

// Компиляция программы
	cl_program build_program(
		// Путь или имя файла в текущем каталоге
			std::string filename, 
		// ID устройства
			cl_device_id device, 
		// Указатель на контекст устройства
			cl_context context);

// Преобразование Фурье на графическом процессоре
	void ocl_fft(
	// 0 - Входная последовательсноть точек
		cl_float2 * x,
	// 1 - Количество точек
		int N,
	// 2 - Количество точек на группу
		int points_per_group);

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
		return -1;

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
		return -1;

	ShowWindow(ghWnd, nCmdShow);
	UpdateWindow(ghWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_QUIT)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

// Преобразование Фурье на графическом процессоре
	void ocl_fft(
		// 0 - Входная последовательсноть точек
			cl_float2 * x,
		// 1 - Количество точек
			int N,
		// 2 - Количество точек на группу
			int points_per_group)
	{
		const int bits = int(log2(double(N)));
		const int nlevels = int(log2(double(N)));
		const int nlevels_per_group = int(log2(double(points_per_group)));
		const int num_groups = N / points_per_group;
	
		cl_device_id device = get_device();
		if (device == nullptr)
			return;
	
		cl_int err = CL_SUCCESS;
		cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	
		if (err != CL_SUCCESS)
			return;
	
		cl_program program = build_program("test.cl", device, context);
		if (program == nullptr)
			return;
	
		cl_kernel kernel = clCreateKernel(program, "testKernel", &err);
		if (err != CL_SUCCESS)
			return;
	
		cl_kernel merge_kernel = clCreateKernel(program, "merge", &err);
		if (err != CL_SUCCESS)
			return;
	
		cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device, nullptr, &err);
		if (err != CL_SUCCESS)
			return;
	
		cl_mem input = clCreateBuffer(
			context,
			CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(cl_float2) * N,
			x, &err);
		if (err != CL_SUCCESS)
			return;
	
		cl_float2 * result = new cl_float2[N];

		cl_mem output = clCreateBuffer(
			context, 
			CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, 
			sizeof(cl_float2) * N, 
			result, &err);
		if (err != CL_SUCCESS)
		{
			delete result;
			return;
		}

		err = clSetKernelArg(kernel, 0, sizeof(input), &input);
		err |= clSetKernelArg(kernel, 1, sizeof(int), &N);
		err |= clSetKernelArg(kernel, 2, sizeof(int), &points_per_group);
		err |= clSetKernelArg(kernel, 3, points_per_group * sizeof(cl_float2), NULL);
		err |= clSetKernelArg(kernel, 4, sizeof(int), &bits);
		err |= clSetKernelArg(kernel, 5, sizeof(int), &nlevels_per_group);
		err |= clSetKernelArg(kernel, 6, sizeof(output), &output);

		std::size_t work_group = num_groups;
		std::size_t work_item = 1;
		err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &work_group, &work_item, 0, NULL, NULL);
		if (err != CL_SUCCESS)
		{
			delete result;
			return;
		}

		err = clSetKernelArg(merge_kernel, 0, sizeof(output), &output);

		int m = points_per_group;

		for (int i = nlevels_per_group + 1; i <= nlevels; i++)
		{
			m <<= 1;
			err |= clSetKernelArg(merge_kernel, 1, sizeof(int), &m);

			if (err != CL_SUCCESS)
			{
				delete result;
				return;
			}

			work_group >>= 1;
			err = clEnqueueNDRangeKernel(command_queue, merge_kernel, 1, NULL, &work_group, &work_item, 0, NULL, NULL);
			if (err != CL_SUCCESS)
			{
				delete result;
				return;
			}
		}

		err = clEnqueueReadBuffer(command_queue, output, CL_TRUE, 0, sizeof(cl_float2) * N, result, 0, NULL, NULL);
		if (err != CL_SUCCESS)
		{
			delete result;
			return;
		}

		std::memcpy(x, result, N * sizeof(cl_float2));
		delete result;
	}

// Генерация синусоидального сигнала
	cl_float2 * generate_sinus(
		// Количество точек
			int N,
		// Амплитуда
			int ampl,
		// Фаза
			int phase,
		// Количество периодов
			int k) 
	{
		if (N == 0 || k == 0)
			return nullptr;

		cl_float2 * x = new cl_float2[N];

		float M = 2 * CL_M_PI_F * k / N;

		for (int n = 0; n < N; n++)
		{
			x[n].x = ((ampl * sinf(M * n + phase)) + 1) / 2;
			x[n].y = 0.0f;
		}

		return x;
	}

// Инициализация графического устройства.
// Определяем платформы на хосте и ищем платформу (первую),
// которая управляет графическим устройством.
	cl_device_id get_device()
	{
		cl_uint num_platforms = 0;
		cl_int err = clGetPlatformIDs(0, nullptr, &num_platforms);
		if (err != CL_SUCCESS)
			return nullptr;
	
		cl_platform_id * platform = new cl_platform_id[num_platforms];
		err = clGetPlatformIDs(num_platforms, platform, nullptr);
		if (err != CL_SUCCESS)
		{
			delete platform;
			return nullptr;
		}

		for (cl_uint i = 0; i < num_platforms; i++)
		{
			cl_uint num_devices = 0;
			err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);
			if (err == CL_SUCCESS && num_devices > 0)
			{
				cl_device_id device;
				num_devices = 1;
				err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_GPU, num_devices, &device, nullptr);
				if (err == CL_SUCCESS)
				{
					delete platform;
					return device;
				}
			}
		}

		delete[] platform;
		return nullptr;
	}

// Компиляция программы
	cl_program build_program(
		// Путь или имя файла в текущем каталоге
			std::string filename,
		// ID устройства
			cl_device_id device,
		// Указатель на контекст устройства
			cl_context context)
	{
		cl_program program;
		std::ifstream file(filename, std::ios_base::binary);
		std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));

		const char* src = code.c_str();
		std::size_t len = code.length() + 1;

		cl_int err;
		program = clCreateProgramWithSource(context, 1, &src, &len, &err);

		if (err != CL_SUCCESS)
			return nullptr;

		err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
		if (err != CL_SUCCESS)
		{
			std::size_t size_log = 0;
			char* build_log;
			err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size_log);
			if (err != CL_SUCCESS) {
				std::cout << "Error (code) - " << err << std::endl;
				return nullptr;
			}

			if (size_log == 0) {
				std::cout << "Error (mess) - not such size_log" << std::endl;
				return nullptr;
			}

			build_log = new char[size_log];
			err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size_log, build_log, nullptr);
			if (err != CL_SUCCESS) {
				std::cout << "Error (code) - " << err << std::endl;
				return nullptr;
			}
			std::cout << build_log << std::endl;
			delete[] build_log;
			return nullptr;
		}

		return program;
	}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {

	case WM_CREATE:
	{
		ghDC = GetDC(hWnd);
		if (!bSetupPixelFormat(ghDC))
			PostQuitMessage(0);

		ghRC = wglCreateContext(ghDC);
		wglMakeCurrent(ghDC, ghRC);

		gen_texture(1024, 1024, &texture);
	}
	break;

	case WM_PAINT:
	{
		BeginPaint(hWnd, &paint);

		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);
		
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

		EndPaint(hWnd, &paint);
	}
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
		return -1;

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

// Генерируем текстуру OpenGL
	void gen_texture(
		// 0 -Высота
			int WIDTH,
		// 1 - ширина
			int HEIGHT,
		// 2 - Текструа
			texture_t * tex)
	{
		cl_float2 * sinus = generate_sinus(HEIGHT, 1, 0, 10);
		if (sinus == nullptr)
			return;

		ocl_fft(sinus, HEIGHT, HEIGHT / 8);

		float* pixels = new float[HEIGHT * WIDTH * 3];
	
		int k = 0;

		for (int i = 0; i < WIDTH; i++)
			for (int j = 0; j < HEIGHT; j++)
			{
				pixels[k++] = sinus[j].x;
				pixels[k++] = sinus[j].x;
				pixels[k++] = sinus[j].x;
			}

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, tex);

		glBindTexture(GL_TEXTURE_2D, *tex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, pixels);

		delete pixels;
		delete sinus;
	}
