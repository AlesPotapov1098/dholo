#include "stdafx.h"
#include "dhimgexpr.h"
#include "resource.h"
#include "dhmainfrm.h"
#include "dholo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

namespace dholo
{
	namespace img
	{

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
			ON_COMMAND(ID_ADD_IMAGE, &DHImgExpr::OnLoadImage)
			ON_COMMAND(ID_DELETE_IMAGE, &DHImgExpr::OnDeleteImage)
			ON_COMMAND(ID_SELECT_IMAGE, &DHImgExpr::OnSelectImage)
			ON_COMMAND(ID_CUT_IMAGE, &DHImgExpr::OnDeleteImage)
			ON_COMMAND(ID_LOAD_INTO_GP, &DHImgExpr::OnLoadIntoGp)
		END_MESSAGE_MAP()

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

			m_FunctionalToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
			m_FunctionalToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);

			m_FunctionalToolBar.SetPaneStyle(m_FunctionalToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

			m_FunctionalToolBar.SetPaneStyle(m_FunctionalToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

			m_FunctionalToolBar.SetOwner(this);

			// Все команды будут перенаправлены через этот элемент управления, а не через родительскую рамку:
			m_FunctionalToolBar.SetRouteCommandsViaFrame(FALSE);

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

		void DHImgExpr::AddImage(const CStringW& pathImage, const CStringW& imageName, const CStringW& imageExt)
		{
			if (!pathImage || !imageName || !imageExt)
				return;

			HRESULT res;
			CImage m_LoaderImageObject;
			CBitmap m_BitmapImageObject;
			res = m_LoaderImageObject.Load(pathImage);

			m_BitmapImageObject.Attach((HBITMAP)m_LoaderImageObject);
			m_ImageList.Add(&m_BitmapImageObject, RGB(0x00, 0x00, 0x00));

			m_dhImgList.SetImageList(&m_ImageList, LVSIL_NORMAL);
			int imageIndex = m_ImageList.Add(&m_BitmapImageObject, RGB(0xff, 0xff, 0xff));

			m_dhImgList.SetImageList(&m_ImageList, LVSIL_SMALL);

			int listIndex;
			LV_ITEM lvItem;
			listIndex = m_dhImgList.GetItemCount() + 1;
			lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
			lvItem.iItem = listIndex;
			lvItem.iSubItem = 0;
			lvItem.iImage = 0;
			lvItem.iImage = imageIndex;
			lvItem.pszText = const_cast<LPWSTR>(imageName.GetString());

			listIndex = m_dhImgList.InsertItem(&lvItem);
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = listIndex;
			lvItem.iSubItem = 1;
			lvItem.iImage = 0;
			lvItem.pszText = const_cast<LPWSTR>(pathImage.GetString());
			m_dhImgList.SetItem(&lvItem);
		}

		void DHImgExpr::OnContextMenu(CWnd* pWnd, CPoint point)
		{
			CListCtrl* pdhImgList = (CListCtrl*)&m_dhImgList;
			ASSERT_VALID(pdhImgList);

			if (pWnd != pdhImgList)
			{
				CDockablePane::OnContextMenu(pWnd, point);
				return;
			}

			if (point != CPoint(-1, -1))
			{
				CPoint ptTree = point;
				pdhImgList->ScreenToClient(&ptTree);
			}

			pdhImgList->SetFocus();
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
		}

		void DHImgExpr::AdjustLayout()
		{
			if (GetSafeHwnd() == nullptr)
			{
				return;
			}

			CRect rectClient;
			GetClientRect(rectClient);

			int cyTlb = m_FunctionalToolBar.CalcFixedLayout(FALSE, TRUE).cy;

			m_FunctionalToolBar.SetWindowPos(
				nullptr, rectClient.left, rectClient.top, rectClient.Width(),
				cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

			m_dhImgList.SetWindowPos(
				nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2,
				rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
		}

		void DHImgExpr::OnPaint()
		{
			CPaintDC dc(this);

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

		void DHImgExpr::OnSelectImage()
		{
			if (!m_dhImgList.GetItemCount())
				return;

			UINT count = m_dhImgList.GetSelectedCount();

			//if (count <= 0 || count > 4 || count < 4)
			//	// TODO : обработка ошибок!!!
			//	return;

			std::vector<CStringA> ImagePaths(count);
			POSITION pos = m_dhImgList.GetFirstSelectedItemPosition();
			if (!pos)
				return;

			for (int i = 0; i < count; i++)
			{
				UINT SelectedItem = m_dhImgList.GetNextSelectedItem(pos);
				ImagePaths[i] = 
					CStringA(m_dhImgList.GetItemText(SelectedItem, 1));
			}

			theApp.SelectImage(ImagePaths);
		}

		void DHImgExpr::OnDeleteImage()
		{
			// TODO: добавьте свой код обработчика команд
		}

		void DHImgExpr::OnLoadImage()
		{
			CFileDialog dlg(TRUE, NULL, L"*.jpg; *.png", OFN_ALLOWMULTISELECT);

			if (dlg.DoModal() == IDOK)
			{
				POSITION ps = dlg.GetStartPosition();
				while (ps)
				{
					CString fileName = dlg.GetFileName();
					CString fileExt = dlg.GetFileExt();
					CString pathName = dlg.GetNextPathName(ps);
					AddImage(pathName, fileName, fileExt);
				}
			}
		}
		
		void DHImgExpr::OnLoadIntoGp()
		{
			// TODO: добавьте свой код обработчика команд
		}

	}
}


