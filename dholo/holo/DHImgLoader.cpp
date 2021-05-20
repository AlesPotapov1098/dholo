#include "pch.h"
#include "DHImgLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace dholo
{
	namespace img
	{
		DHImgLoader::DHImgLoader()
		{
			m_PixelsData = nullptr;
			m_width = 0;
			m_height = 0;
			m_nrChannels = 0;
		}

		DHImgLoader::~DHImgLoader()
		{
			if (m_PixelsData)
				stbi_image_free(m_PixelsData);
		}

		bool DHImgLoader::Load(const CStringA& imgPath)
		{
			stbi_set_flip_vertically_on_load(true);
			m_PixelsData = stbi_loadf(imgPath.GetString(), &m_width, &m_height, &m_nrChannels, 0);

			if (!m_PixelsData)
				throw dholo::exp::DHAppExp("Fail to load pixels");

			return true;
		}

		int DHImgLoader::GetWidth() const
		{
			return m_width;
		}

		int DHImgLoader::GetHeight() const
		{
			return m_height;
		}

		int DHImgLoader::GetChannels() const
		{
			return m_nrChannels;
		}

		float* DHImgLoader::GetPixelsData() const
		{
			return m_PixelsData;
		}

		void DHImgLoader::GenerateImage(int width, int height, int channels)
		{
			m_width = width;
			m_height = height;
			m_nrChannels = channels;

			m_PixelsData = new float[width * height * channels];
			for (int i = 0; i < width * height * channels;)
				for (int j = 0; j < channels; j++)
					m_PixelsData[i++] = 1.0f;
		}
		void DHImgLoader::SetPixels(int width, int height, int channles, float * pxls)
		{
			if (m_PixelsData)
				delete[] m_PixelsData;

			m_width = width;
			m_height = height;
			m_nrChannels = channles;

			std::size_t size = m_width * m_height * m_nrChannels;

			m_PixelsData = new float[size];
			
			for (int i = 0; i < size; i++)
				m_PixelsData[i] = pxls[i];
		}
	}
}
