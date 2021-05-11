#pragma once

#include <vector>

#include "DHImgLoader.h"
#include "DHOCLTransform.h"

enum class OperationType
{
	/// Рисуем только одно изображение из буфера
	DH_OPERATION_NONE = 0,
	/// Рисуем красный квадрат
	DH_OPERATION_PAINT_IT_RED = 1,
	/// Обрабатываем 4 изображения
	DH_OPERATION_COMPLEX_CALCULATION = 2,
	/// Проводим тестовый запуск
	DH_OPERATION_TEST = 3
};

class DHWnd : public CWnd
{
public:
	DHWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~DHWnd();
	void OpenDlg();
	void LoadTexture(const std::vector<CStringA>& path);
	void GenerateTest();
	void LoadImg(const CStringA& imgPath);
	void LoadImg(const std::vector<CStringA>& imgPaths);
	void GenSin();
	afx_msg int OnCreate(LPCREATESTRUCT lpcst);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSaveImg();
	DECLARE_MESSAGE_MAP()

private:
	OperationType m_OperType;
	std::vector<dholo::img::DHImgLoader> m_ImgLoader;

	int m_GlobalSizeX, m_GlobalSizeY;
	int m_LocalSizeX, m_LocalSizeY;
	dholo::gpgpu::DHGPGPUTransform *m_Transform;

	PAINTSTRUCT m_Paint;
	CDC* m_pDC;
	RECT m_Rect;
	CRect m_CRect;
public:
	afx_msg void OnSaveAndAddImg();
};

