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

		private:
			float* m_PixelsData;

			int m_width;
			int m_height;
			int m_nrChannels;
		};
	}
}
