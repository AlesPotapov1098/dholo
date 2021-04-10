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
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// обработчики сообщений CWorkspaceBar

int DHImgExpr::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Создание представления:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_ICON | LVS_REPORT;

	if (!m_dhImgList.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Не удалось создать представление файлов\n");
		return -1;      // не удалось создать
	}
	
	m_ImageList.Create(64, 64, ILC_COLOR24 | ILC_MASK, 8, 4);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// Все команды будут перенаправлены через этот элемент управления, а не через родительскую рамку:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Введите некоторые данные статического представления в виде дерева (пустой код, ничего более)
	FillFileView();
	AdjustLayout();

	return 0;
}

void DHImgExpr::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void DHImgExpr::FillFileView()
{
	int Index;
	BOOL validString;
	
	CString columnImage;
	validString = columnImage.LoadStringW(IDS_IMAGE_LIST_COLUMN_IMAGE);
	if (!validString)
		return;
	
	m_dhImgList.InsertColumn(0, columnImage, LVCFMT_LEFT);
	
	CString columnPath;
	validString = columnPath.LoadStringW(IDS_IMAGE_LIST_COLUMN_PATH);
	if (!validString)
		return;
	
	m_dhImgList.InsertColumn(1, columnPath, LVCFMT_LEFT);
	
	m_dhImgList.SetColumnWidth(0, COLUMN_IMAGE_WIDTH);
	m_dhImgList.SetColumnWidth(1, COLUMN_PATH_WIDTH);
}

void DHImgExpr::AddImage(CStringW pathImage, CStringW imageName, CStringW imageExt)
{
	//if (!pathImage || !imageName || !imageExt)
	//	return;
	//HRESULT res;
	//CImage m_LoaderImageObject;
	//CBitmap m_BitmapImageObject;
	//res = m_LoaderImageObject.Load(pathImage);
	//
	//m_BitmapImageObject.Attach((HBITMAP)m_LoaderImageObject);
	//m_ImageList.Add(&m_BitmapImageObject, RGB(0x00, 0x00, 0x00));
	//
	//m_List.SetImageList(&m_ImageList, LVSIL_NORMAL);
	//int imageIndex = m_ImageList.Add(&m_BitmapImageObject, RGB(0xff, 0xff, 0xff));
	//
	//m_List.SetImageList(&m_ImageList, LVSIL_SMALL);
	//
	//int listIndex;
	//LV_ITEM lvItem;
	//listIndex = m_List.GetItemCount() + 1;
	//lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
	//lvItem.iItem = listIndex;
	//lvItem.iSubItem = 0;
	//lvItem.iImage = 0;
	//lvItem.iImage = imageIndex;
	//lvItem.pszText = const_cast<LPWSTR>(imageName.GetString());
	//
	//listIndex = m_List.InsertItem(&lvItem);
	//lvItem.mask = LVIF_TEXT;
	//lvItem.iItem = listIndex;
	//lvItem.iSubItem = 1;
	//lvItem.iImage = 0;
	//lvItem.pszText = const_cast<LPWSTR>(pathImage.GetString());
	//m_List.SetItem(&lvItem);
}

void DHImgExpr::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//CTreeCtrl* pWndTree = (CTreeCtrl*)&m_List;
	//ASSERT_VALID(pWndTree);
	//
	//if (pWnd != pWndTree)
	//{
	//	CDockablePane::OnContextMenu(pWnd, point);
	//	return;
	//}
	//
	//if (point != CPoint(-1, -1))
	//{
	//	// Выбрать нажатый элемент:
	//	CPoint ptTree = point;
	//	pWndTree->ScreenToClient(&ptTree);
	//
	//	UINT flags = 0;
	//	HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
	//	if (hTreeItem != nullptr)
	//	{
	//		pWndTree->SelectItem(hTreeItem);
	//	}
	//}
	//
	//pWndTree->SetFocus();
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void DHImgExpr::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_dhImgList.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void DHImgExpr::OnProperties()
{
	OnLoadImage();
}

void DHImgExpr::OnFileOpen()
{
	OnLoadImage();
}

void DHImgExpr::OnFileOpenWith()
{

}

void DHImgExpr::OnCreateNew()
{
	//if (!m_List.GetItemCount())
	//	return;
	//
	//POSITION pos = m_List.GetFirstSelectedItemPosition();
	//
	//if (pos == 0)
	//	return;
	//
	//UINT SelectedItem = m_List.GetNextSelectedItem(pos);
	//CString ImagePath = m_List.GetItemText(SelectedItem, 1);
	//
	//theApp.OnFileNew();
	//
	//DigitalHologramFrame* pMdiActiveFrame =
	//	static_cast<DigitalHologramFrame*>(
	//		static_cast<CMainFrame*>(theApp.m_pMainWnd)->MDIGetActive());
	//
	//if (!pMdiActiveFrame)
	//	return;
	//
	//if (!pMdiActiveFrame->IsKindOf(RUNTIME_CLASS(DigitalHologramFrame)))
	//	return;
	//
	//CDigitalHologramView* mHoloView =
	//	static_cast<CDigitalHologramView*>(pMdiActiveFrame->GetActiveView());
	//
	//if (!mHoloView)
	//	return;
	//
	//if (!mHoloView->IsKindOf(RUNTIME_CLASS(CDigitalHologramView)))
	//	return;
	//
	//mHoloView->SetImagePath(ImagePath);
}

void DHImgExpr::OnEditCut()
{
	// TODO: добавьте сюда код обработчика команд
}

void DHImgExpr::OnEditCopy()
{
	// TODO: добавьте сюда код обработчика команд
}

void DHImgExpr::OnEditClear()
{
	// TODO: добавьте сюда код обработчика команд
}

void DHImgExpr::OnPaint()
{
	CPaintDC dc(this); // контекст устройства для рисования

	CRect rectTree;
	m_dhImgList.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void DHImgExpr::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_dhImgList.SetFocus();
}

void DHImgExpr::OnChangeVisualStyle()
{
	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);
	//
	//m_ImageList.DeleteImageList();
	//
	//UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;
	//
	//CBitmap bmp;
	//if (!bmp.LoadBitmap(uiBmpId))
	//{
	//	TRACE(_T("Не удается загрузить точечный рисунок: %x\n"), uiBmpId);
	//	ASSERT(FALSE);
	//	return;
	//}
	//
	//BITMAP bmpObj;
	//bmp.GetBitmap(&bmpObj);

	//UINT nFlags = ILC_MASK;
	//
	//nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;
}

void DHImgExpr::OnLoadImage()
{
	CFileDialog dlg(true);

	if (dlg.DoModal() == IDOK)
		AddImage(dlg.GetPathName(), dlg.GetFileName(), dlg.GetFileExt());
}

void DHImgExpr::OnUpdateDummyCompile(CCmdUI* pCmdUI)
{
	//if (m_List.GetItemCount() == 0)
	//{
	//	pCmdUI->Enable(false);
	//	return;
	//}

	//if (!m_List.GetSelectedCount())
	//{
	//	pCmdUI->Enable(false);
	//	return;
	//}
	//
	//pCmdUI->Enable(true);
}	//
