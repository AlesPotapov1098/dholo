#pragma once

#include <Windows.h> 
#include <GL/gl.h> 

#define CL_HPP_TARGET_OPENCL_VERSION 220
#include <CL/cl2.h>

#include <string>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

/// <summary>
/// Переделать при переносе в основной проект
/// </summary>
class GPGPU
{
public:
	GPGPU();
	~GPGPU();

	void InitOpenGL(HDC hDC);
	void InitOpenCL();

	void UpdateOpenGL(HDC hDC);

	void Execute();
	void DrawScene();

	void ReleaseOpenGL();
	void ReleaseOpenCL();

// Общие страктуры данных
private:
	HDC m_hDC;

// OpenGL структуры данных
private:
	PIXELFORMATDESCRIPTOR m_PixelFormat;
	HGLRC m_hRC;
	GLuint * m_Texture;
	int m_CountTextures;
	int m_TexWidth;
	int m_TexHeight;

// OpenCL структуры данных
private:
	cl_platform_id* m_Platform;
	cl_device_id* m_Device;
	cl_context m_Context;
	cl_program m_Program;
	cl_kernel m_Kernel;
	cl_command_queue m_CommandQueue;

	cl_int m_ErrorCode;
};

