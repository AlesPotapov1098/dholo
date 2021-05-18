#pragma once

#include "DHOCLTransform.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHGPGPUGetPrmtrs : public DHGPGPUTransform
		{
		public:

			DHGPGPUGetPrmtrs() = default;
			DHGPGPUGetPrmtrs(GLuint* tex);
			~DHGPGPUGetPrmtrs() = default;
			
			void Init(const DHOCLHost&, HDC dc, HGLRC rc) override;
			virtual void Calculate(int global_w, int global_h, int local_w, int local_h) override;
			virtual void Release() override;

		private:
			cl_mem m_Mem;
			GLuint* m_Tex;
		};
	}
}
