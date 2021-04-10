#pragma once

#include <Windows.h>

#include <gl/GLU.h>
#include <gl/GL.h>

#include <iostream>

#pragma comment(lib, "OpenGL32.lib")

namespace gpgpu {
	namespace ogl {
		class OGL
		{
		public:
			OGL();
			~OGL();

			void InitOpenGL(HDC);

			bool IsActive();
			void Render(CDC* dc);

			void InitTexture(CString pathImage);

			void ReleaseOpenGL();

		private:
			HDC m_hDC;
			HGLRC m_hRC;
			PIXELFORMATDESCRIPTOR m_Desc;

			CImage m_Image;
			CBitmap m_Bitmap;
			BITMAP m_hBitmap;

			GLuint m_Texture;

			float m_CubeCoord[4][2];
			float m_TexCoord[4][2];

			int WIDTH, HEIGHT;

			GLuint* pixels;
			GLuint texture;
		};
	}
}