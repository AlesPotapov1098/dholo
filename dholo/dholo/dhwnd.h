#pragma once

#include "dhogl.h"
#include "dhimgloader.h"
#include "dhoclcalc.h"
#include "dhoclinit.h"

namespace dholo
{
	namespace holo
	{
		enum OperationType
		{
			DH_OPERATION_NONE = 0,
			/// Рисуем красный квадрат
			DH_OPERATION_PAINT_IT_RED = 1,
		};

		class DHWnd : public CWnd
		{
		public:
			DHWnd();
			virtual ~DHWnd();

			void OpenDlg();
			void LoadTexture(const CStringA& path);

		protected:
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

			afx_msg int OnCreate(LPCREATESTRUCT lpcst);
			afx_msg void OnPaint();
			afx_msg void OnDestroy();
			afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

			DECLARE_MESSAGE_MAP()

		private:
			ogl::DHOGLRender mogl;
			ocl::DHOCLCalc m_calc;
			OperationType m_OperType;

			img::DHImgLoader m_ImgLoader;
			ocl::DHOCLHost m_Host;

			PAINTSTRUCT m_Paint;
			CDC* m_pDC;
			RECT m_Rect;
		public:
			afx_msg void OnConvertImage();
		};
	}
}