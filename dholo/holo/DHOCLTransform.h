#pragma once

#include <afxwin.h>
#include <Windows.h>
#include <string>
#include <gl\GLU.h>
#include <gl\GL.h>
#include <fstream>
#include <vector>
#include <cmath>

#include "DHOCLInit.h"
#include "DHImgLoader.h"
#include "DHGPGPUExp.h"

#include <CL\cl.h>
#include <CL\cl_gl.h>

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUTransform
		{
		public:
			DHGPGPUTransform();
			DHGPGPUTransform(GLuint* tex, const dholo::img::DHImgLoader& img);
			virtual ~DHGPGPUTransform();

			/// <summary>
			/// Инициализация OpenGL и OpenCL
			/// </summary>
				virtual void Init(const DHOCLHost&, HDC, HGLRC);

			/// <summary>
			/// Преобразование
			/// </summary>
				virtual void Calculate(int global_w, int global_h, int local_w, int local_h);

			/// <summary>
			/// Освобождение ресурсов
			/// </summary>
				virtual void Release();

			/// <summary>
			/// Задаем путь к файлу программы
			/// </summary>
			/// <param name="">Путь к файлу</param>
			void SetProgramName(const std::string&);

			/// <summary>
			/// Задаем название ядра
			/// </summary>
			/// <param name="">Название ядра</param>
			void SetKernelName(const std::string&);

		protected:
			cl_program m_Program;
			cl_kernel m_Kernel;
			cl_context m_Context;
			cl_mem m_InOutMem;
			cl_command_queue m_CommandQueue;
			std::string m_ProgramPath;
			std::string m_KernelName;
			dholo::gpgpu::DHOCLHost m_Host;

			GLuint* m_Tex;
			dholo::img::DHImgLoader m_ImgLdr;
		};
	}
}
