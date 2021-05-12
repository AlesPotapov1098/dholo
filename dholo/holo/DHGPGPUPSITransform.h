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
			DHGPGPUPSITransform(const std::vector<dholo::img::DHImgLoader>& imgs, float phases[4], float B);
			virtual ~DHGPGPUPSITransform();

			void Init(const CDC&, const DHOCLHost&) override;
			void SetImages(const std::vector<dholo::img::DHImgLoader>&);
			void GenerateTexture() override;
			void Calculate() override;
			void Release() override;
			void RenderScene() override;

		private:
			std::vector<dholo::img::DHImgLoader> m_Images;
			float m_Phases[4];
			float m_B;
			GLuint m_Textures[5];
			cl_mem m_Mem[5];
		};
	}
}

