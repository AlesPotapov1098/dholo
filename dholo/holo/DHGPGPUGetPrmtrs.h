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
			
			void Init(const CDC&, const DHOCLHost&) override;
			void GenerateTexture() override;
			void Calculate() override;
			void RenderScene() override;
			void Release() override;

		protected:
			cl_mem m_Mem;
		};
	}
}
