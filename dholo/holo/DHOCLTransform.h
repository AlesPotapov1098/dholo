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
			virtual ~DHGPGPUTransform();

			/// <summary>
			/// ������������� OpenGL � OpenCL
			/// </summary>
				virtual void Init(const CDC&, const DHOCLHost&);
			
			/// <summary>
			/// ��������� �������
			/// </summary>
				virtual void GenerateTexture();

			/// <summary>
			/// ��������������
			/// </summary>
				virtual void Calculate();

			/// <summary>
			/// ������������ ��������
			/// </summary>
				virtual void Release();

			/// <summary>
			/// ��������� �����
			/// </summary>
				virtual void RenderScene();
		protected:
			void InitOpenCL(const DHOCLHost&);

		protected:
			cl_program m_Program;
			cl_kernel m_Kernel;
			cl_context m_Context;
			cl_mem m_InOutMem;
			cl_command_queue m_CommandQueue;
			HGLRC m_hRC;
			HDC m_hDC;
			PIXELFORMATDESCRIPTOR m_Desc;
			std::string m_ProgramPath;
			std::string m_KernelName;
			dholo::gpgpu::DHOCLHost m_Host;
		};

		class test : public DHGPGPUTransform
		{
		public:
			test() = default;
			~test() = default;
			void RenderScene() override
			{
				glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
				DHGPGPUTransform::RenderScene();
			}
		};

		

		
	}
}
