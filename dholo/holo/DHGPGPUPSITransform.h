#pragma once

#include "DHOCLTransform.h"

namespace dholo
{
	namespace gpgpu
	{
		/// <summary>
		/// Структура для хранения параметров PSI преобразования
		/// </summary>
		struct PSIStruct
		{
			CString m_ImgNames[4];
			float m_Phases[4];

			float B;
		};

		/// <summary>
		/// Реализация PSI преобразования
		/// </summary>
		class DHGPGPUPSITransform : public DHGPGPUTransform
		{
		public:
			DHGPGPUPSITransform();
			DHGPGPUPSITransform(GLuint* tex, const PSIStruct& psi, img::DHImgLoader * img);
			virtual ~DHGPGPUPSITransform();

			void Init(const DHOCLHost&, HDC dc, HGLRC rc) override;
			void Calculate(int global_w, int global_h, int local_w, int local_h) override;
			void Release() override;

		private:
			PSIStruct m_PSISettings;

			img::DHImgLoader* m_img;

			dholo::img::DHImgLoader m_Images[5];
			GLuint m_Textures[5];
			GLuint* m_RenderTex;
			cl_mem m_InputImg[5];
		};
	}
}

