#pragma once

#include <gl\GL.h>

#include <string>
#include <codecvt>

class DHImgLoader
{
public:
	DHImgLoader();
	~DHImgLoader();

	bool Load(const CStringA& imgPath);
	bool OpenDialog();

	int GetWidth() const;
	int GetHeight() const;
	int GetChannels() const;

	unsigned char * GetPixelsData() const;

private:
	unsigned char * m_PixelsData;
	LPSTR m_ImagePath;

	int m_width;
	int m_height;
	int m_nrChannels;
};

