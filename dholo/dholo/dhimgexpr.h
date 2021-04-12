#pragma once

#include "dhimglist.h"
#include "dhtoolbar.h"

class DHImgExpr : public CDockablePane
{
	// Создание
public:
	DHImgExpr() noexcept;
	virtual ~DHImgExpr();

	void AdjustLayout();
	void OnChangeVisualStyle();
	void AddImage(CStringW pathImage, CStringW imageName, CStringW imageExt);

protected:
	void FillFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnCreateNew();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

private:
	DHToolBar m_wndToolBar;
	DHImgList m_dhImgList;
	CImageList m_ImageList;

	const int COLUMN_IMAGE_WIDTH = 120;
	const int COLUMN_PATH_WIDTH = 90;
public:
	afx_msg void OnUpdateDummyCompile(CCmdUI* pCmdUI);
	afx_msg void OnDeleteImage();
	afx_msg void OnLoadImage();
};

