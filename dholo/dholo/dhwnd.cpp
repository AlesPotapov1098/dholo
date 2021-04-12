#include "stdafx.h"
#include "dholo.h"
#include "dhwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DHWnd::DHWnd()
{
	ZeroMemory(&m_Desc, sizeof(PIXELFORMATDESCRIPTOR));
}

DHWnd::~DHWnd()
{
}

void DHWnd::OpenDlg()
{
	CFileDialog dlg(true);
	if (dlg.DoModal() == IDOK)
		m_ogl.InitTexture(CStringA(dlg.GetPathName()));
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
	m_ogl.Render(m_pDC, &CRect(m_Rect));
	EndPaint(&m_Paint);
}

int DHWnd::OnCreate(LPCREATESTRUCT lpcst)
{
	m_ogl.InitOpenGL();
	return CWnd::OnCreate(lpcst);
}

void DHWnd::OnDestroy()
{
	m_ogl.ReleaseOpenGL();
}

void DHWnd::LoadTexture(const CStringA& path)
{
	m_ogl.InitTexture(path);
}
