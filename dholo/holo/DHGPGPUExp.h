#pragma once

#include "DHAppExp.h"

#include <CL/cl.h>

namespace dholo
{
	namespace exp
	{
		class DHGPGPUExp : public DHAppExp
		{
		public:
			DHGPGPUExp(cl_int error_code);
			~DHGPGPUExp();
		};
	}
}
