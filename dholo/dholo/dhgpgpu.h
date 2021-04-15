#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <CL/cl.h>

#include "dhimgloader.h"

class DHGpGPU
{
public:
	DHGpGPU();
	~DHGpGPU();

// Общие данные
private:
	HDC m_hDC;

// Данные для OpenGL
private:
	PIXELFORMATDESCRIPTOR m_PixelFormat;
	HGLRC m_hGLRC;

	GLuint* m_Texture;

	int m_Width;
	int m_Height;
	int m_Channels;

// Данные для OpenCL
private:
	cl_context m_Context;
	cl_program m_Program;
	cl_kernel m_Kernel;
	cl_command_queue m_CommandQueue;

	cl_int m_ErrorCode;
};
