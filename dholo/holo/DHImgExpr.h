#pragma once

#include "DHImgList.h"
#include "DHToolbar.h"

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
			void AddImage(const CStringW& pathImage, const CStringW& imageName, const CStringW& imageExt);

			afx_msg void OnDeleteImage();
			afx_msg void OnLoadImage();
			afx_msg void OnLoadIntoGp();
			afx_msg void OnShowImage();

		protected:
			void FillFileView();
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
			afx_msg void OnPaint();
			afx_msg void OnSetFocus(CWnd* pOldWnd);
			afx_msg void OnSelectImage();

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

