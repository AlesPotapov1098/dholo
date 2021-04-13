#pragma once

#include <Windows.h>

#include <gl/GLU.h>
#include <gl/GL.h>

#include <iostream>

#include "dhimgloader.h"
#include "idhrender.h"

#pragma comment(lib, "OpenGL32.lib")

class OGL1 : public IDHRender
{
public:
	OGL1();
	~OGL1();

	/// <summary>
	/// Захват контекста и создание контекста OpenGL для Windows.
	/// </summary>
	void virtual Init(const CDC& dc);
	
	/// <summary>
	/// Создание текстуры из пикселей imgldr
	/// </summary>
	void virtual LoadImg(const DHImgLoader& imgldr, const CRect& rect);

	/// <summary>
	/// Рисуем
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Даем текстуру
	/// </summary>
	GLuint GetTexture() const;

private:
	HGLRC m_hRC;
	PIXELFORMATDESCRIPTOR m_Desc;
	GLuint m_Texture;

	GLfloat m_X;
	GLfloat m_Y;
};

namespace gpgpu {
	namespace ogl {
		class OGL
		{
		public:
			OGL();
			~OGL();

			void InitOpenGL();

			bool IsActive();
			void Render(CDC* dc, CRect* rect);

			void LoadTexture(const CStringA& pathImage);
			void InitTexture();
			void DrawTexture();
			void ReleaseOpenGL();

		private:
			HGLRC m_hRC;
			PIXELFORMATDESCRIPTOR m_Desc;

			GLuint m_Texture;

			GLfloat m_X;
			GLfloat m_Y;

			CRect * m_Rect;

			DHImgLoader m_ImgLoader;
		};
	}
}