#pragma once

#define W	1024
#define H	1024

#define _USE_MATH_DEFINES
#include <cmath>

namespace holo
{
	namespace test {
		class DHTest
		{
		public:
			DHTest();
			~DHTest();

			void GenerateTextExmpl();

			float * GetTestExmpl(int index);

		private:
			const int m_W = 1024;
			const int m_H = 1024;

			float m_Pixels[5][W * H];
			float m_Sinus[4][W];
		};
	}
}