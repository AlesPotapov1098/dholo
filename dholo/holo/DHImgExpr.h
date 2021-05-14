#pragma once

#include "DHImgList.h"
#include "DHToolbar.h"
#include "DHOCLTransDlg.h"

namespace dholo
{
	namespace img
	{
		/// <summary>
		/// ��������� ������ ��� PSI ��������������
		/// </summary>
		struct PSISettings
		{

		};

		class DHImgExpr : public CDockablePane
		{
			// ��������
		public:
			DHImgExpr() noexcept;
			virtual ~DHImgExpr();

			void AdjustLayout();

			void DeleteSelectedImg();
			CString GetSelectedImg();
			void AddImage(const CStringW& pathImage, const CStringW& imageName, const CStringW& imageExt);

			afx_msg void OnShowImage();

			/// <summary>
			/// �������� 4-� ����������� ��� PSI ��������������
			/// </summary>
				void SelectImages(dholo::gpgpu::PSIStruct*psi);

			void FillFileView();
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
			afx_msg void OnPaint();
			afx_msg void OnSetFocus(CWnd* pOldWnd);

			DECLARE_MESSAGE_MAP()

		private:
			UINT GetSelecteIndex() const;

		private:
			toolbar::DHToolBar m_FunctionalToolBar;
			DHImgList m_dhImgList;
			CImageList m_ImageList;

			const int COLUMN_IMAGE_WIDTH = 120;
			const int COLUMN_PATH_WIDTH = 90;
		};
	}
}

