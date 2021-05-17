#pragma once

#include "DHGPGPUGetPrmtrs.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUGetAmpl : public DHGPGPUGetPrmtrs
		{
		public:

			DHGPGPUGetAmpl() = default;
			DHGPGPUGetAmpl(const img::DHImgLoader& img);
			~DHGPGPUGetAmpl() = default;
		};
	}
}
