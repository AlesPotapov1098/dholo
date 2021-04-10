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


BEGIN_MESSAGE_MAP(DHWnd, CWnd)
	ON_WM_PAINT()
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
	CPaintDC dc(this); 
}

