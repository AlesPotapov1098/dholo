#include "stdafx.h"

#include "dhtest.h"

namespace holo
{	namespace test
	{
		DHTest::DHTest()
		{
		}

		DHTest::~DHTest()
		{
		}

		void DHTest::GenerateTextExmpl()
		{
			float phi1 = 0,
				phi2 = M_PI / 2,
				phi3 = M_PI,
				phi4 = 3 * M_PI / 2;

			int K = 10;

			int k = 0;

			for (int i = 0; i < W; i++)
			{
				m_Sinus[0][i] = (sinf((2 * M_PI * K / W) * i + phi1) + 1) / 2;
				m_Sinus[1][i] = (sinf((2 * M_PI * K / W) * i + phi2) + 1) / 2;
				m_Sinus[2][i] = (sinf((2 * M_PI * K / W) * i + phi3) + 1) / 2;
				m_Sinus[3][i] = (sinf((2 * M_PI * K / W) * i + phi4) + 1) / 2;
			}

			for (int i = 0; i < H; i++)
			{
				for (int j = 0; j < W; j++)
				{
					float n1 = m_Sinus[0][j],
						  n2 = m_Sinus[1][j],
						  n3 = m_Sinus[2][j],
						  n4 = m_Sinus[3][j];

					m_Pixels[0][k] = n1;
					m_Pixels[1][k] = n2;
					m_Pixels[2][k] = n3;
					m_Pixels[3][k] = n4;
					m_Pixels[4][k] = 0.0f;

					k++;

					m_Pixels[0][k] = n1;
					m_Pixels[1][k] = n2;
					m_Pixels[2][k] = n3;
					m_Pixels[3][k] = n4;
					m_Pixels[4][k] = 0.0f;

					k++;

					m_Pixels[0][k] = n1;
					m_Pixels[1][k] = n2;
					m_Pixels[2][k] = n3;
					m_Pixels[3][k] = n4;
					m_Pixels[4][k] = 0.0f;

					k++;
				}
			}
		}

		float * DHTest::GetTestExmpl(int index)
		{
			if (index >= 5)
				return nullptr;

			return m_Pixels[index];
		}
	}
}