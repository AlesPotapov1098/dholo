#pragma once

#include <gl\GL.h>

#include <string>
#include <codecvt>
#include <vector>

#include "DHAppExp.h"

namespace dholo
{
	namespace img
	{
		class DHImgLoader
		{
		public:
			DHImgLoader();
			~DHImgLoader();

			bool Load(const CStringA& imgPath);

			int GetWidth() const;
			int GetHeight() const;
			int GetChannels() const;

			float* GetPixelsData() const;

			void GenerateImage(int width, int height, int channels);
			void SetPixels(int width, int height, int channles, float *pxls);

		private:
			float* m_PixelsData;

			int m_width;
			int m_height;
			int m_nrChannels;
		};
	}
}
