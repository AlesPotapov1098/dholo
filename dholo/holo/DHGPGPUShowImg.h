#pragma once

#include "DHOCLTransform.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUShowImg : public DHGPGPUTransform
		{
		public:
			DHGPGPUShowImg(const dholo::img::DHImgLoader& img);
			~DHGPGPUShowImg();

			virtual void GenerateTexture() override;
			virtual void RenderScene() override;

		private:
			dholo::img::DHImgLoader m_ImgLdr;
			GLuint m_Texture;
		};
	}
}
