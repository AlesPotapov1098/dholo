#pragma once

#include "DHGPGPUGetPrmtrs.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUGetIm : public DHGPGPUGetPrmtrs
		{
		public:

			DHGPGPUGetIm() = default;
			DHGPGPUGetIm(const dholo::img::DHImgLoader& img);
			~DHGPGPUGetIm() = default;
		};
	}
}
