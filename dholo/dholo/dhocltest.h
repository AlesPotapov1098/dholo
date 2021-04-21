#pragma once

#include "dhoclcalc.h"
#include "dhoclinit.h"
#include "dhogl.h"

namespace dholo
{
	namespace test
	{
		class DHOCLTest : public ocl::DHOCLCalc
		{
		public:
			DHOCLTest();
			DHOCLTest(const ocl::DHOCLHost& host, const std::string& progpath);
			virtual ~DHOCLTest();

			void LoadImg(const ogl::DHOGLRender& scene);
			void Calculate();
		private:
			cl_mem m_Mem[5];
		};
	}
}