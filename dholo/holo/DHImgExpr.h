#pragma once

#include "DHImgList.h"
#include "DHToolbar.h"
#include "DHOCLTransDlg.h"

namespace dholo
{
	namespace img
	{
		class DHImgExpr : public CDockablePane
		{
			// Создание
		public:
			DHImgExpr() noexcept;
			virtual ~DHImgExpr();

			void AdjustLayout();

			UINT GetSelecteIndex() const;
			UINT GetSelectedImageCount() const;
			void DeleteSelectedImg();
			CString GetSelectedImg();
			void AddImage(const CStringW& pathImage, const CStringW& imageName, const CStringW& imageExt);

			afx_msg void OnShowImage();

			/// <summary>
			/// Фиксация 4-х изображений для PSI преобразования
			/// </summary>
				void SelectImages(dholo::gpgpu::PSIStruct *psi);

			void FillFileView();
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
			afx_msg void OnPaint();
			afx_msg void OnSetFocus(CWnd* pOldWnd);
			afx_msg void OnPSITransform();
			afx_msg void OnAddImage();
			afx_msg void OnDeleteImage();

			DECLARE_MESSAGE_MAP()

		private:
			toolbar::DHToolBar m_FunctionalToolBar;
			DHImgList m_dhImgList;
			CImageList m_ImageList;

			const int COLUMN_IMAGE_WIDTH = 120;
			const int COLUMN_PATH_WIDTH = 90;
		};
	}
}

