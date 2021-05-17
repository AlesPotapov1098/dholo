#pragma once

#include "DHGPGPUGetPrmtrs.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUGetRe : public DHGPGPUGetPrmtrs
		{
		public:

			DHGPGPUGetRe() = default;
			DHGPGPUGetRe(const img::DHImgLoader& img);
			~DHGPGPUGetRe() = default;
		};
	}
}
