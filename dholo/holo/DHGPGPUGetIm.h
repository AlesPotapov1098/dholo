#pragma once

#include "DHGPGPUShowImg.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUGetIm : public DHGPGPUShowImg
		{
		public:

			DHGPGPUGetIm() = default;
			DHGPGPUGetIm(const dholo::img::DHImgLoader& img);
			~DHGPGPUGetIm() = default;
		};
	}
}
