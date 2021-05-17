#pragma once

#include "DHGPGPUShowImg.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUGetPrmtrs : public DHGPGPUShowImg
		{
		public:

			DHGPGPUGetPrmtrs() = default;
			DHGPGPUGetPrmtrs(const dholo::img::DHImgLoader& img);
			~DHGPGPUGetPrmtrs() = default;
			
			virtual void Init(const CDC&, const DHOCLHost&) override;
			virtual void GenerateTexture() override;
			virtual void Calculate() override;
			virtual void RenderScene() override;
			virtual void Release() override;

		private:
			cl_mem m_Mem;
		};
	}
}
