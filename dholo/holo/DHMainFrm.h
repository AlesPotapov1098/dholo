#pragma once
#include "DHWnd.h"
#include "DHToolbar.h"
#include "DHImgExpr.h"
#include "DHOCLTransform.h"
#include "DHAppExp.h"

#include <vector>

class DHMainFrm : public CFrameWndEx
{
	
public:
	DHMainFrm() noexcept;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
	void SelectImage(const std::vector<CStringA>& imgPath);
	void LoadImg(const CStringA& imgPath);
	void LoadImg(const std::vector<CStringA>& imgPaths);
	virtual ~DHMainFrm();

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDeleteImage();
	afx_msg void OnAddImage();
	afx_msg void OnLoadIntoGp();
	afx_msg void OnPSITransform();
	
	void OnMenu(CPoint point);
	
	DECLARE_DYNAMIC(DHMainFrm)

public:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:  // встроенные члены панели элементов управления
	CMFCToolBar       m_wndToolBar;

	CMFCMenuBar       m_wndMenuBar;
	dholo::toolbar::DHToolBar       m_FunctionalToolBar;
	dholo::toolbar::DHToolBar m_TransformToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	DHWnd    m_targetWnd;

	dholo::img::DHImgExpr m_imgList;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnGenSin();
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSaveImg();
	afx_msg void OnSaveAndAddImg();
};


