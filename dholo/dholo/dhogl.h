#pragma once

#include <Windows.h>

#include <gl/GLU.h>
#include <gl/GL.h>

#include <iostream>

#include "dhimgloader.h"
#include "idhrender.h"

#pragma comment(lib, "OpenGL32.lib")

namespace dholo
{
	namespace ogl
	{
		class DHOGLRender : public IDHRender
		{
		public:
			DHOGLRender();
			~DHOGLRender();

			/// <summary>
			/// ������ ��������� � �������� ��������� OpenGL ��� Windows.
			/// </summary>
			void Init(const CDC& dc);

			/// <summary>
			/// �������� �������� �� �������� imgldr
			/// </summary>
			void LoadImg(const dholo::img::DHImgLoader& imgldr, const CRect& rect);

			/// <summary>
			/// �������� �������� �� �������� imgldr
			/// </summary>
			void LoadImg(const std::vector<dholo::img::DHImgLoader>& imgldr);

			/// <summary>
			/// ������
			/// </summary>
			void Draw();

			/// <summary>
			/// ���� ��������
			/// </summary>
			GLuint GetTexture() const;

		private:
			HGLRC m_hRC;
			PIXELFORMATDESCRIPTOR m_Desc;
			GLuint m_Texture;

			GLfloat m_X;
			GLfloat m_Y;
		};
	}
}
