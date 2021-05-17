#pragma once

#include "DHGPGPUGetPrmtrs.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUGetPhase : public DHGPGPUGetPrmtrs
		{
		public:

			DHGPGPUGetPhase() = default;
			DHGPGPUGetPhase(const img::DHImgLoader& img);
			~DHGPGPUGetPhase() = default;
		};
	}
}
