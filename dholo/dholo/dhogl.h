#pragma once

#include <Windows.h>

#include <gl/GLU.h>
#include <gl/GL.h>

#include <iostream>

#include "dhimgloader.h"

#pragma comment(lib, "OpenGL32.lib")

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

			void InitTexture(const CStringA& pathImage);
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