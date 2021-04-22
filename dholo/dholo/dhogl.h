#pragma once

#include <Windows.h>

#include <gl/GLU.h>
#include <gl/GL.h>

#include <iostream>

#include "dhimgloader.h"
#include "idhrender.h"
#include "dhtest.h"

#pragma comment(lib, "OpenGL32.lib")

namespace dholo
{
	namespace ogl
	{
		class DHOGLRender
		{
		public:
			DHOGLRender();
			DHOGLRender(const CDC& dc);
			~DHOGLRender();

			/// <summary>
			/// ������������� OpenGL � ������� ���������
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
			/// ����
			/// </summary>
			void LoadImg(const test::DHTest& test);

			/// <summary>
			/// ������� �������� � ��������� �������
			/// </summary>
			void LoadTexture(const std::vector<dholo::img::DHImgLoader>& imgldr, const CRect& rect);

			/// <summary>
			/// ������
			/// </summary>
			void Draw();

			/// <summary>
			/// ������
			/// </summary>
			void Draw(unsigned int index);

			/// <summary>
			/// ������
			/// </summary>
			void DrawTestAll();

			void DrawTest();

			/// <summary>
			/// ���� ��������
			/// </summary>
			GLuint GetTexture() const;

			/// <summary>
			/// ��������� ��������
			/// </summary>
			GLuint GetTexture(unsigned int index) const;

		private:
			HGLRC m_hRC;
			PIXELFORMATDESCRIPTOR m_Desc;
			GLuint* m_Texture;
			UINT m_Cnt;

			GLfloat m_X;
			GLfloat m_Y;
		};
	}
}
