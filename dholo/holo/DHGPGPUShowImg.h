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

			virtual void GenerateTexture();
			virtual void Release();
			virtual void RenderScene();

		private:
			dholo::img::DHImgLoader m_ImgLdr;
		};
	}
}
