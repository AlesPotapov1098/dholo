#pragma once

#include "DHOCLTransform.h"

namespace dholo
{
	namespace gpgpu
	{
		/// <summary>
		/// Реализация PSI преобразования
		/// </summary>
		class DHGPGPUPSITransform : public DHGPGPUTransform
		{
		public:
			DHGPGPUPSITransform();
			virtual ~DHGPGPUPSITransform();

			void Init(const CDC&, const DHOCLHost&) override;
			void SetImages(const std::vector<dholo::img::DHImgLoader>&);
			void GenerateTexture() override;
			void Calculate() override;
			void Release() override;
			void RenderScene() override;

		private:
			std::vector<dholo::img::DHImgLoader> m_Images;
			GLuint m_Textures[5];
			cl_mem m_Mem[5];
		};
	}
}

