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
			DHGPGPUPSITransform(const PSIStruct& psi);
			virtual ~DHGPGPUPSITransform();

			void Init(const CDC&, const DHOCLHost&) override;
			void GenerateTexture() override;
			void Calculate() override;
			void Release() override;
			void RenderScene() override;

		private:
			PSIStruct m_PSISettings;

			dholo::img::DHImgLoader m_Images[5];
			GLuint m_Textures[5];
			cl_mem m_Mem[5];
		};
	}
}

