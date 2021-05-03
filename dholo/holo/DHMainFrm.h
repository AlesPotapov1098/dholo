#pragma once
#include "DHWnd.h"
#include "DHToolbar.h"
#include "DHImgExpr.h"

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
protected: 
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
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	DHWnd    m_wndView;

	dholo::img::DHImgExpr m_imgExpr;

// Созданные функции схемы сообщений
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

};


