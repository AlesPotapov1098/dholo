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
			/// Инициализация OpenGL в целевом контексте
			/// </summary>
			void Init(const CDC& dc);

			/// <summary>
			/// Создание текстуры из пикселей imgldr
			/// </summary>
			void LoadImg(const dholo::img::DHImgLoader& imgldr, const CRect& rect);

			/// <summary>
			/// Создание текстуры из пикселей imgldr
			/// </summary>
			void LoadImg(const std::vector<dholo::img::DHImgLoader>& imgldr);

			/// <summary>
			/// Тест
			/// </summary>
			void LoadImg(const test::DHTest& test);

			/// <summary>
			/// Создаем текстуры и загружаем пиксели
			/// </summary>
			void LoadTexture(const std::vector<dholo::img::DHImgLoader>& imgldr, const CRect& rect);

			/// <summary>
			/// Рисуем
			/// </summary>
			void Draw();

			/// <summary>
			/// Рисуем
			/// </summary>
			void Draw(unsigned int index);

			/// <summary>
			/// Рисуем
			/// </summary>
			void DrawTestAll();

			void DrawTest();

			/// <summary>
			/// Даем текстуру
			/// </summary>
			GLuint GetTexture() const;

			/// <summary>
			/// Возвращем текстуру
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
