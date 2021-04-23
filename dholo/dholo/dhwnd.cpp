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
			cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC,
				::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

			return TRUE;
		}

		void DHWnd::OnPaint()
		{
			m_pDC = BeginPaint(&m_Paint);
			GetWindowRect(&m_Rect);

			ogl::DHOGLRender m_OGL(*m_pDC);

			switch (m_OperType)
			{
			case OperationType::DH_OPERATION_NONE:
				if(!m_ImgLoader.empty())
					m_OGL.LoadTexture(m_ImgLoader, &CRect(m_Rect));
				m_OGL.DrawTest();
				break;
			
			case OperationType::DH_OPERATION_PAINT_IT_RED:
			{
				ocl::DHOCLCalc m_calc(m_Host, "black.cl");
				m_OGL.LoadImg(m_ImgLoader[0], &CRect(m_Rect));
				m_calc.LoadImg(m_OGL.GetTexture());
				m_calc.SetGlobalSize(m_ImgLoader[0].GetWidth(), m_ImgLoader[0].GetHeight());
				m_calc.SetLocalSize(1, 1);
				m_calc.Calculate();
				m_OGL.Draw();
			}
				break;
			
			case OperationType::DH_OPERATION_COMPLEX_CALCULATION:
				if (!m_ImgLoader.empty())
					m_OGL.LoadTexture(m_ImgLoader, &CRect(m_Rect));
				m_OGL.DrawTestAll();
				break;

			case OperationType::DH_OPERATION_TEST:
			{
				dholo::test::DHOCLTest m_cals(m_Host, "Psi4Kernel.cl");
				m_OGL.LoadImg(m_Test);
				m_cals.LoadImg(m_OGL);
				m_cals.SetGlobalSize(1024, 1024);
				m_cals.SetLocalSize(1, 1);
				m_cals.Calculate();
				m_OGL.Draw(4);
			}
				break;

			default:
				m_OGL.Draw();
				break;
			}

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
			else if(path.size() == 1)
				m_OperType = OperationType::DH_OPERATION_NONE;

			int len = path.size();

			m_ImgLoader.resize(len);

			for (int i = 0; i < len; i++)
				m_ImgLoader[i].Load(path[i]);
		}

		void DHWnd::GenerateTest()
		{
			dlg::DHOpenCLDlg dlg;
			if (dlg.DoModal() != IDOK)
				return;

			m_Host = dlg.GetOpenCLHost();
			m_OperType = OperationType::DH_OPERATION_TEST;

			m_Test.GenerateTextExmpl();

			Invalidate();
			UpdateWindow();
		}

		void DHWnd::LoadImg(const CStringA & imgPath)
		{
			if (imgPath.IsEmpty())
				/// TODO: обработка ошибок
				return;

			m_OperType = OperationType::DH_OPERATION_NONE;

			m_ImgLoader.clear();
			m_ImgLoader.resize(1);
			m_ImgLoader[0].Load(imgPath);
		}

		void DHWnd::LoadImg(const std::vector<CStringA>& imgPaths)
		{
			if(imgPaths.empty())
				/// TODO: обработка ошибок
				return;

			m_OperType = OperationType::DH_OPERATION_COMPLEX_CALCULATION;

			m_ImgLoader.clear();
			m_ImgLoader.resize(4);

			for (int i = 0; i < 4; i++)
				m_ImgLoader[i].Load(imgPaths[i]);
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