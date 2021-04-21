#include "stdafx.h"

#include "dhtest.h"

namespace dholo
{	namespace test
	{
		DHTest::DHTest()
		{
			m_Pixels = new float*[5];
			for (int i = 0; i < 5; i++)
				m_Pixels[i] = new float[W * H * 3];
		}

		DHTest::~DHTest()
		{
			if (m_Pixels)
				delete[] m_Pixels;
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
					m_Pixels[0][k] = m_Sinus[0][j];
					m_Pixels[1][k] = m_Sinus[1][j];
					m_Pixels[2][k] = m_Sinus[2][j];
					m_Pixels[3][k] = m_Sinus[3][j];
					m_Pixels[4][k] = 0.0f;

					k++;

					m_Pixels[0][k] = m_Sinus[0][j];
					m_Pixels[1][k] = m_Sinus[1][j];
					m_Pixels[2][k] = m_Sinus[2][j];
					m_Pixels[3][k] = m_Sinus[3][j];
					m_Pixels[4][k] = 0.0f;

					k++;

					m_Pixels[0][k] = m_Sinus[0][j];
					m_Pixels[1][k] = m_Sinus[1][j];
					m_Pixels[2][k] = m_Sinus[2][j];
					m_Pixels[3][k] = m_Sinus[3][j];
					m_Pixels[4][k] = 0.0f;

					k++;
				}
			}
		}

		float * DHTest::GetTestExmpl(int index) const
		{
			if (index >= 5)
				return nullptr;

			return m_Pixels[index];
		}

		int DHTest::GetWidth() const
		{
			return m_W;
		}

		int DHTest::GetHeight() const
		{
			return m_H;
		}
	}
}