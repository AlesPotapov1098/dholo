#include "stdafx.h"
#include "dhimgexpr.h"
#include "resource.h"
#include "dhmainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

DHImgExpr::DHImgExpr() noexcept
{
}

DHImgExpr::~DHImgExpr()
{
}

BEGIN_MESSAGE_MAP(DHImgExpr, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//ON_COMMAND(ID_ADD_IMAGE, OnAddImage)
END_MESSAGE_MAP()

void DHImgExpr::AddJustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_toolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_toolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void DHImgExpr::OnChangeVisualStyle()
{
}

int DHImgExpr::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_toolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_toolBar.LoadToolBar(IDR_EXPLORER);

	//m_toolBar.SetPaneStyle(m_toolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	//m_toolBar.SetPaneStyle(m_toolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_toolBar.SetOwner(this);

//	m_toolBar.SetRouteCommandsViaFrame(FALSE);

	AddJustLayout();

	return 0;
}

void DHImgExpr::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void DHImgExpr::OnContextMenu(CWnd * pWnd, CPoint point)
{

}

void DHImgExpr::OnProperties()
{

}

void DHImgExpr::OnAddImage()
{

}

void DHImgExpr::OnPaint()
{
	CPaintDC dc(this); // контекст устройства для рисования

	CRect rectTree;
	//m_List.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void DHImgExpr::OnSetFocus(CWnd * pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
}
