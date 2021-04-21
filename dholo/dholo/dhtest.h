#pragma once

#define W	1024
#define H	1024

#define _USE_MATH_DEFINES
#include <cmath>

namespace dholo
{
	namespace test {
		class DHTest
		{
		public:
			DHTest();
			~DHTest();

			void GenerateTextExmpl();

			float * GetTestExmpl(int index);

			int GetWidth() const;
			int GetHeight() const;

		private:
			const int m_W = 1024;
			const int m_H = 1024;

			float m_Pixels[5][W * H * 3];
			float m_Sinus[4][W];
		};
	}
}