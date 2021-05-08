#pragma once

#include <afxwin.h>
#include <Windows.h>
#include <string>
#include <gl\GLU.h>
#include <gl\GL.h>
#include <fstream>
#include <vector>

#include "DHOCLInit.h"

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
			virtual ~DHGPGPUTransform();

			// Инициализация OpenGL и OpenCL
				virtual void Init(const CDC&, const DHOCLHost&);
			
			// Генерация текстур
				virtual void GenerateTexture();

			// Преобразование
				virtual void Calculate();

			// Освобождение ресурсов
				virtual void Release();

			// Отрисовка сцены
				virtual void RenderScene();

		protected:
			cl_program m_Program;
			cl_kernel m_Kernel;
			cl_context m_Context;
			cl_mem m_InOutMem;
			cl_command_queue m_CommandQueue;
			HGLRC m_hRC;
			HDC m_DC;
			PIXELFORMATDESCRIPTOR m_Desc;
		};

		class test : public DHGPGPUTransform
		{
		public:
			test() = default;// { DHOCLTestTransform::DHOCLTestTransform(); }
			~test() = default;
			void RenderScene() override
			{
				glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
				DHGPGPUTransform::RenderScene();
			}
		};

		class DHGPGPUPSITransform : public DHGPGPUTransform
		{
		};
	}
}
