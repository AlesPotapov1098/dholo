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
			DHGPGPUGenSinus(GLuint *tex, int width, int height, float ampl, float phi, int T);
			virtual ~DHGPGPUGenSinus();

			void Calculate(int global_w, int global_h, int local_w, int local_h) override;

		private:
			float m_amp;
			float m_phase;
			int m_width;
			int m_height;
			int m_T;

			GLuint* m_SinTexture;
		};
	}
}