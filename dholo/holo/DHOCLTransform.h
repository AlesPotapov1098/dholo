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
	namespace ocl
	{
		class DHOCLTestTransform
		{
		public:
			DHOCLTestTransform();
			virtual ~DHOCLTestTransform();

			// Инициализация OpenGL и OpenCL
				void Init(const CDC&, const DHOCLHost&);
			
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

		class DHOCLTransform
		{
		public:
			DHOCLTransform();
			DHOCLTransform(const DHOCLHost& host, const std::string& progpath);
			~DHOCLTransform();

			/// <summary>
			/// Загрузка текстуры OpenGL.
			/// Инициализация объекта m_InOutMem
			/// </summary>
			void LoadImg(const GLuint& texture);

			/// <summary>
			/// Установка размера глобальной группы
			/// </summary>
			void SetGlobalSize(std::size_t global_x, std::size_t global_y);

			/// <summary>
			/// Установка размера глобальной группы
			/// </summary>
			void SetGlobalSize(std::size_t globalSize[2]);

			/// <summary>
			/// Установка размера локальной группы
			/// </summary>
			void SetLocalSize(std::size_t local_x, std::size_t local_y);

			/// <summary>
			/// Установка размера локальной группы
			/// </summary>
			void SetLocalSize(std::size_t localSize[2]);

			/// <summary>
			/// Передача ядру m_Kernel входных значений.
			/// Выполнение ядра с заданными параметрами
			/// </summary>
			void Calculate();

		protected:
			cl_program m_Program;
			cl_kernel m_Kernel;
			cl_context m_Context;
			cl_mem m_InOutMem;
			cl_command_queue m_CommandQueue;

			std::size_t m_GlobalSize[2];
			std::size_t m_LocalSize[2];
		};
	}
}
