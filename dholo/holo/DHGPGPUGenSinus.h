#pragma once

#include "DHOCLTransform.h"

namespace dholo
{
	namespace gpgpu
	{
		/// <summary>
		/// Генерация синусоидального сигнала
		/// </summary>
		class DHGPGPUGenSinus : public DHGPGPUTransform
		{
		public:
			DHGPGPUGenSinus(int width, int height, float ampl, float phi, int T);
			virtual ~DHGPGPUGenSinus();

			void GenerateTexture() override;
			void Calculate() override;
			void Release() override;
			void RenderScene() override;

		private:
			float m_amp;
			float m_phase;
			int m_width;
			int m_height;
			int m_T;

			GLuint m_SinTexture;
		};
	}
}