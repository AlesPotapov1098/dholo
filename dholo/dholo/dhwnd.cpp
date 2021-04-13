#include "stdafx.h"
#include "dholo.h"
#include "dhwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DHWnd::DHWnd()
{
}

DHWnd::~DHWnd()
{
}

void DHWnd::OpenDlg()
{
	CFileDialog dlg(true);
	if (dlg.DoModal() == IDOK)
		m_ImgLoader.Load(CStringA(dlg.GetPathName()));
}

BEGIN_MESSAGE_MAP(DHWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL DHWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void DHWnd::OnPaint() 
{
	m_pDC = BeginPaint(&m_Paint);
	GetWindowRect(&m_Rect);
	mogl.Init(*m_pDC);
	mogl.LoadImg(m_ImgLoader, &CRect(m_Rect));
	mogl.Draw();
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

void DHWnd::LoadTexture(const CStringA& path)
{
	m_ImgLoader.Load(path);
}
