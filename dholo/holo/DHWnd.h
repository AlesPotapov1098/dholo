#pragma once

#include <vector>

#include "DHImgLoader.h"
#include "DHOCLTransform.h"
#include "DHGPGPUGenSinus.h"
#include "DHGPGPUPSITransform.h"
#include "DHAppExp.h"
#include "DHGPGPUGetPrmtrs.h"
#include "DHOCLTransDlg.h"

struct ImageFile
{
	bool isOK;
	CString path;
	CString ext;
	CString name;
};

class DHWnd : public CWnd
{
public:
	DHWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~DHWnd();
	void OpenDlg();

	/// <summary>
	/// Загрузка изображения в графический процессор
	/// </summary>
	/// <param name="imgPath">Целевое изображение</param>
		void LoadImg(const CStringA& imgPath);

	/// <summary>
	/// Генерация синуса
	/// </summary>
		void GenSin();

	/// <summary>
	/// PSI преобразрвание
	/// </summary>
	/// <param name="psi">Параметры для PSI преобразования</param>
	/// <param name="host">Хост</param>
		void PSITransform(const dholo::gpgpu::PSIStruct& psi, const dholo::gpgpu::DHOCLHost &host);

	ImageFile OnSaveImg();

	void OnGetAmplitude();
	void OnGetPhase();
	void OnGetRe();
	void OnGetIm();

	void InitHandles();
	void Render();

	afx_msg int OnCreate(LPCREATESTRUCT lpcst);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	dholo::img::DHImgLoader m_ImgLoader;

	int m_GlobalSizeX, m_GlobalSizeY;
	int m_LocalSizeX, m_LocalSizeY;
	dholo::gpgpu::DHGPGPUTransform *m_Transform;

	dholo::gpgpu::DHOCLHost m_Host;

	PAINTSTRUCT m_Paint;
	CDC* m_pDC;
	HGLRC m_hRC;
	PIXELFORMATDESCRIPTOR m_Desc;
	RECT m_Rect;
	CRect m_CRect;

	GLuint m_RenderTargetTexture;
	bool f = false;
};

