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

#define NUM_POINTS				16
#define NUM_POINTS_PER_GROUP	4
#define NUM_GROUPS				(NUM_POINTS / NUM_POINTS_PER_GROUP)

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

const int bits = int(log2(double(NUM_POINTS)));
const int nlevels = int(log2(double(NUM_POINTS)));
const int nlevels_per_group = int(log2(double(NUM_POINTS_PER_GROUP)));

cl_float2 global[NUM_POINTS];

void Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

void Render();
void GenSinus();

void ortho(float * res, float * vec);
void mul(float * res, float * a, float * b);
void fft();

int bit_reversed(int x, int bits) {
	int y = 0;
	for (int i = 0; i < bits; i++) {
		y <<= 1;
		y |= x & 1;
		x >>= 1;
	}
	y &= ((1 << bits) - 1);
	return y;
}

// Алгоритм перебора бабочка
	void butterfly(int * right);

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

int main()
{
	for (int i = 0; i < NUM_POINTS; i++)
	{
		global[i].x = i;
		global[i].y = 0;
	}

	cl_device_id device = get_device();
	if (device == nullptr)
		return -1;
	
	cl_int err = CL_SUCCESS;
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	
	if (err != CL_SUCCESS)
		return -1;
	
	cl_program program = build_program("test.cl", device, context);
	if (program == nullptr)
		return -1;
	
	cl_kernel kernel = clCreateKernel(program, "testKernel", &err);
	if (err != CL_SUCCESS)
		return -1;
	
	cl_kernel merge_kernel = clCreateKernel(program, "merge", &err);
	if (err != CL_SUCCESS)
		return -1;
	
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device, nullptr, &err);
	if (err != CL_SUCCESS)
		return -1;

	std::size_t size = sizeof(cl_float2);
	
	cl_mem input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2) * NUM_POINTS, global, &err);
	if (err != CL_SUCCESS)
		return -1;
	
	cl_float2 result[NUM_POINTS];
	
	cl_mem output = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_float2) * NUM_POINTS, result, &err);
	if (err != CL_SUCCESS)
		return -1;
	
	int n = NUM_POINTS;
	int m = NUM_POINTS_PER_GROUP;
	
	err = clSetKernelArg(kernel, 0, sizeof(input), &input);
	err |= clSetKernelArg(kernel, 1, sizeof(int), &n);
	err |= clSetKernelArg(kernel, 2, sizeof(int), &m);
	err |= clSetKernelArg(kernel, 3, NUM_POINTS_PER_GROUP * sizeof(cl_float2), NULL);
	err |= clSetKernelArg(kernel, 4, sizeof(int), &bits);
	err |= clSetKernelArg(kernel, 5, sizeof(int), &nlevels_per_group);
	err |= clSetKernelArg(kernel, 6, sizeof(output), &output);
	
	std::size_t work_group = NUM_GROUPS;
	std::size_t work_item = 1;
	err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &work_group, &work_item, 0, NULL, NULL);
	if (err != CL_SUCCESS)
		return -1;
	
	err = clSetKernelArg(merge_kernel, 0, sizeof(output), &output);
	
	for (int i = nlevels_per_group + 1; i <= nlevels; i++)
	{
		m <<= 1;
		err |= clSetKernelArg(merge_kernel, 1, sizeof(int), &m);
	
		if (err != CL_SUCCESS)
			return -1;
	
		work_group >>= 1;
		err = clEnqueueNDRangeKernel(command_queue, merge_kernel, 1, NULL, &work_group, &work_item, 0, NULL, NULL);
		if (err != CL_SUCCESS)
			return -1;
	}
	
	err = clEnqueueReadBuffer(command_queue, output, CL_TRUE, 0, sizeof(cl_float2) * NUM_POINTS, result, 0, NULL, NULL);
	if (err != CL_SUCCESS)
		return -1;

	fft();

	return 0;
}

// Алгоритм перебора бабочка
	//void butterfly(int* right)
	//{
	//	int** local = new int*[NUM_GROUPS];
	//	for (int i = 0; i < NUM_GROUPS; i++)
	//		local[i] = new int[NUM_POINTS_PER_GROUP];
	//
	//	for (int i = 0; i < NUM_GROUPS; i++)
	//	{
	//		int start_index = i * NUM_POINTS_PER_GROUP;
	//		for (int j = 0; j < NUM_POINTS_PER_GROUP; j++)
	//		{
	//			int index = bit_reversed(j + start_index, bits);
	//			local[i][j] = global[index];
	//		}
	//	}
	//
	//	int step = 1;
	//	int offset = 1;
	//	int cycles = 1;
	//
	//	for (int g = 0; g < NUM_GROUPS; g++)
	//	{
	//		for (int l = 1; l <= nlevels_per_group; l++)
	//		{
	//			step *= 2;
	//
	//			for (int i = 0; i < NUM_POINTS_PER_GROUP; i += step)
	//			{
	//				for (int j = 0; j < cycles; j++)
	//				{
	//					int a = local[g][i + j];
	//					int b = local[g][i + j + offset];
	//					local[g][i + j] = a + b;
	//					local[g][i + j + offset] = a - b;
	//				}
	//			}
	//
	//			offset *= 2;
	//			cycles *= 2;
	//		}
	//
	//		step = 1;
	//		offset = 1;
	//		cycles = 1;
	//	}
	//
	//	bool res = 0;
	//	for (int i = 0; i < NUM_GROUPS; i++)
	//	{
	//		for (int j = 0; j < NUM_POINTS_PER_GROUP; j++)
	//		{
	//			res |= (local[i][j] == right[i * NUM_POINTS_PER_GROUP + j]);
	//			//std::cout << output[i*M + j] << std::endl;
	//		}
	//	}
	//
	//	if (res)
	//		std::cout << "Success" << std::endl;
	//	else
	//		std::cout << "Fail" << std::endl;
	//
	//	delete [] local;
	//}

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

		for (int i = 0; i < num_platforms; i++)
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
	float phi1 = 0,
		phi2 = CL_M_PI / 2,
		phi3 = CL_M_PI,
		phi4 = 3 * CL_M_PI / 2;

	int K = 10;

	int k = 0;

	for (int i = 0; i < WIDTH; i++)
	{
		sinus[0][i] = (sinf((2 * CL_M_PI * K / WIDTH) * i + phi1) + 1) / 2;
		sinus[1][i] = (sinf((2 * CL_M_PI * K / WIDTH) * i + phi2) + 1) / 2;
		sinus[2][i] = (sinf((2 * CL_M_PI * K / WIDTH) * i + phi3) + 1) / 2;
		sinus[3][i] = (sinf((2 * CL_M_PI * K / WIDTH) * i + phi4) + 1) / 2;
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
	int m = 4;
	int i, j, ip, k, l;
	int n = (int)pow(2., m); 
	int n1 = n >> 1;
	
	for (i = 0, j = 0, k = n1; i<n - 1; i++, j = j + k)
	{
		if (i<j) 
		{ 
			cl_float2 t = global[j]; 
			global[j] = global[i];
			global[i] = t;
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
				cl_float2 t;
				t.x = global[ip].x * U_Re - global[ip].y * U_Im;
				t.y = global[ip].x * U_Im + global[ip].y * U_Re;
	
				global[ip].x = global[i].x - t.x;
				global[ip].y = global[i].y - t.y;
	
				global[i].x = global[i].x + t.x;
				global[i].y = global[i].y + t.y;
			}
	
			U_Re = U_Re * W_Re - U_Im * W_Im;
			U_Im = U_Re * W_Im + U_Im * W_Re;
		}
	}
}
