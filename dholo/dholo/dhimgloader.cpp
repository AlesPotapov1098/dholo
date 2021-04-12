#include "stdafx.h"
#include "dhimgloader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

DHImgLoader::DHImgLoader()
{
}


DHImgLoader::~DHImgLoader()
{
	if(m_PixelsData)
		stbi_image_free(m_PixelsData);
}

bool DHImgLoader::Load(const CStringA& imgPath)
{
	if (!imgPath)
		return false;

	stbi_set_flip_vertically_on_load(true);
	m_PixelsData = stbi_load(imgPath.GetString(), &m_width, &m_height, &m_nrChannels, 0);
	if (!m_PixelsData)
		return false;

	return true;
}

bool DHImgLoader::OpenDialog()
{
	CFileDialog dlg(true);
	if (dlg.DoModal() == IDOK)
	{
		Load(CStringA(dlg.GetPathName()));
		return true;
	}
	
	return false;
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

unsigned char * DHImgLoader::GetPixelsData() const
{
	return m_PixelsData;
}
