#pragma once

class DHImgExprToolBar : public CToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class DHImgExpr : public CDockablePane
{
// Создание
public:
	DHImgExpr() noexcept;
	~DHImgExpr();

	void AddJustLayout();
	void OnChangeVisualStyle();
	
// Переопределение функций базовго класса
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnAddImage();

	DECLARE_MESSAGE_MAP()

// Приватные члены для создания 
private:
	DHImgExprToolBar m_toolBar;

// Приватные члены для работы с изображениями
private:
};

