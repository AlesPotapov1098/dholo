#include "stdafx.h"
#include "dholo.h"
#include "dhwnd.h"
#include "dhopencldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace dholo
{
	namespace holo
	{
		DHWnd::DHWnd()
		{
			m_OperType = OperationType::DH_OPERATION_NONE;
		}

		DHWnd::~DHWnd()
		{
		}

		void DHWnd::OpenDlg()
		{
			CFileDialog dlg(true);
			if (dlg.DoModal() != IDOK)
				/// TODO : обработка ошибок!!!
				return;

			if (!m_ImgLoader.empty())
				m_ImgLoader.clear();
			test.Load(CStringA(dlg.GetPathName()));
			m_ImgLoader.resize(1);
			m_ImgLoader[0].Load(CStringA(dlg.GetPathName()));
			m_OperType = OperationType::DH_OPERATION_NONE;
		}

		BEGIN_MESSAGE_MAP(DHWnd, CWnd)
			ON_WM_PAINT()
			ON_WM_CREATE()
			ON_WM_TIMER()
			ON_WM_DESTROY()
			ON_WM_CONTEXTMENU()
			ON_COMMAND(ID_CONVERT_IMAGE, &DHWnd::OnConvertImage)
		END_MESSAGE_MAP()

		BOOL DHWnd::PreCreateWindow(CREATESTRUCT& cs)
		{
			if (!CWnd::PreCreateWindow(cs))
				return FALSE;

			cs.dwExStyle |= WS_EX_CLIENTEDGE;
			cs.style &= ~WS_BORDER;
			cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
				::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

			return TRUE;
		}

		void DHWnd::OnPaint()
		{
			m_pDC = BeginPaint(&m_Paint);
			GetWindowRect(&m_Rect);

			//switch (m_OperType)
			//{
			//case OperationType::DH_OPERATION_NONE:
				mogl.Init(*m_pDC);
					mogl.LoadImg(test, &CRect(m_Rect));
				mogl.Draw();
			//	break;
			//
			//case OperationType::DH_OPERATION_PAINT_IT_RED:
			//	mogl.Init(*m_pDC);
			//	m_calc.Init(m_Host, "black.cl");
			//	mogl.LoadImg(m_ImgLoader[0], &CRect(m_Rect));
			//	m_calc.LoadImg(mogl.GetTexture());
			//	m_calc.SetGlobalSize(m_ImgLoader[0].GetWidth(), m_ImgLoader[0].GetHeight());
			//	m_calc.SetLocalSize(1, 1);
			//	m_calc.Calculate();
			//	m_calc.Release();
			//	mogl.Draw();
			//	break;
			//
			//case OperationType::DH_OPERATION_COMPLEX_CALCULATION:
			//	mogl.Init(*m_pDC);
			//	mogl.LoadImg(m_ImgLoader);
			//	mogl.DrawAll();
			//	break;
			//}

			SwapBuffers(m_pDC->m_hDC);
			EndPaint(&m_Paint);
		}

		int DHWnd::OnCreate(LPCREATESTRUCT lpcst)
		{
			return CWnd::OnCreate(lpcst);
		}

		void DHWnd::OnDestroy()
		{
			CWnd::OnDestroy();
		}

		void DHWnd::OnContextMenu(CWnd* pWnd, CPoint point)
		{
			if (point != CPoint(-1, -1))
			{
				CPoint ptTree = point;
				pWnd->ScreenToClient(&ptTree);
			}

			pWnd->SetFocus();
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_CONVERTER, point.x, point.y, this, TRUE);
		}

		void DHWnd::LoadTexture(const std::vector<CStringA>& path)
		{
			if (!m_ImgLoader.empty())
				m_ImgLoader.clear();

			if (path.size() > 1)
				m_OperType = OperationType::DH_OPERATION_COMPLEX_CALCULATION;

			int len = path.size();

			m_ImgLoader.resize(len);

			for (int i = 0; i < len; i++)
				m_ImgLoader[i].Load(path[i]);
		}


		void DHWnd::OnConvertImage()
		{
			dlg::DHOpenCLDlg dlg;
			if (dlg.DoModal() != IDOK)
				return;

			m_Host = dlg.GetOpenCLHost();
			m_OperType = OperationType::DH_OPERATION_PAINT_IT_RED;
			
			Invalidate();
			UpdateWindow();
		}
	}
}