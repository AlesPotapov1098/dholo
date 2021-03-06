#include "pch.h"
#include "DHImgExpr.h"
#include "DHMainFrm.h"
#include "DHApp.h"

#include "resource.h"

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
			ON_COMMAND(ID_PSI_TRANSFORM, &DHImgExpr::OnPSITransform)
			ON_COMMAND(ID_TOOLBAR_ADD_IMAGE, &DHImgExpr::OnAddImage)
			ON_COMMAND(ID_TOOLBAR_DELETE_IMAGE, &DHImgExpr::OnDeleteImage)
		END_MESSAGE_MAP()

		int DHImgExpr::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (CDockablePane::OnCreate(lpCreateStruct) == -1)
				return -1;

			CRect rectDummy;
			rectDummy.SetRectEmpty();

			// ???????? ?????????????:
			const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_ICON | LVS_REPORT;

			if (!m_dhImgList.Create(dwViewStyle, rectDummy, this, 4))
			{
				TRACE0("?? ??????? ??????? ????????????? ??????\n");
				return -1;      // ?? ??????? ???????
			}

			m_ImageList.Create(64, 64, ILC_COLOR24 | ILC_MASK, 8, 4);
			
			m_FunctionalToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
			m_FunctionalToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ???????????? */);

			m_FunctionalToolBar.SetPaneStyle(m_FunctionalToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

			m_FunctionalToolBar.SetPaneStyle(m_FunctionalToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

			m_FunctionalToolBar.SetOwner(this);

			// ??? ??????? ????? ?????????????? ????? ???? ??????? ??????????, ? ?? ????? ???????????? ?????:
			m_FunctionalToolBar.SetRouteCommandsViaFrame(FALSE);

			// ??????? ????????? ?????? ???????????? ????????????? ? ???? ?????? (?????? ???, ?????? ?????)
			FillFileView();
			AdjustLayout();

			return 0;
		}

		void DHImgExpr::OnSize(UINT nType, int cx, int cy)
		{
			CDockablePane::OnSize(nType, cx, cy);
			AdjustLayout();
		}

		void DHImgExpr::SelectImages(dholo::gpgpu::PSIStruct* psi)
		{
			POSITION pos = m_dhImgList.GetFirstSelectedItemPosition();
			
			if (!pos)
				throw dholo::exp::DHAppExp("?????? ??? ??????????? ???????");

			for (int i = 0; i < 4; i++)
			{
				UINT SelectedItem = m_dhImgList.GetNextSelectedItem(pos);
				psi->m_ImgNames[i] = m_dhImgList.GetItemText(SelectedItem, 1);
			}
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
				throw dholo::exp::DHAppExp("Invalid image file paramter(-s)");

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
			static_cast<DHMainFrm*>(this->GetParent())->OnMenu(point);
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

		UINT DHImgExpr::GetSelectedImageCount() const
		{
			return m_dhImgList.GetSelectedCount();
		}

		void DHImgExpr::DeleteSelectedImg()
		{
			m_dhImgList.DeleteItem(GetSelecteIndex());
		}

		CStringA DHImgExpr::GetSelectedImg()
		{
			return static_cast<CStringA>(m_dhImgList.GetItemText(GetSelecteIndex(), 1));
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

		void DHImgExpr::OnPSITransform()
		{
			static_cast<DHMainFrm*>(this->GetParent())->OnPSITransform();
		}

		void DHImgExpr::OnAddImage()
		{
			static_cast<DHMainFrm*>(this->GetParent())->OnAddImage();
		}

		UINT DHImgExpr::GetSelecteIndex() const
		{
			UINT selectedItem = m_dhImgList.GetSelectedCount();
			if (selectedItem == 0)
				throw dholo::exp::DHAppExp(L"?????????? ??????? ???????????!");

			POSITION pos = m_dhImgList.GetFirstSelectedItemPosition();

			return m_dhImgList.GetNextSelectedItem(pos);
		}

		void DHImgExpr::OnDeleteImage()
		{
			static_cast<DHMainFrm*>(this->GetParent())->OnDeleteImage();
		}
	}
}