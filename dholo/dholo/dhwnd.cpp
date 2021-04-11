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

BEGIN_MESSAGE_MAP(DHWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
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
	Render();
	EndPaint(&m_Paint);
}

int DHWnd::OnCreate(LPCREATESTRUCT lpcst)
{
	InitOpenGL();
	InitSpaceCord();
	return CWnd::OnCreate(lpcst);
}

void DHWnd::InitOpenGL()
{
	m_Desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	m_Desc.nVersion = 1;
	m_Desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	m_Desc.iPixelType = PFD_TYPE_RGBA;
	m_Desc.cColorBits = 24;
	m_Desc.cDepthBits = 32;
	m_Desc.iLayerType = PFD_MAIN_PLANE;
}

void DHWnd::InitSpaceCord()
{
	m_CubeCoord[0][0] = -0.5f;	m_TexCoord[0][0] = 0.0f;
	m_CubeCoord[0][1] = -0.5f;	m_TexCoord[0][1] = 0.0f;

	m_CubeCoord[1][0] = 0.5f;	m_TexCoord[1][0] = 1.0f;
	m_CubeCoord[1][1] = -0.5f;	m_TexCoord[1][1] = 0.0f;

	m_CubeCoord[2][0] = 0.5f;	m_TexCoord[2][0] = 1.0f;
	m_CubeCoord[2][1] = 0.5f;	m_TexCoord[2][1] = 1.0f;

	m_CubeCoord[3][0] = -0.5f;	m_TexCoord[3][0] = 0.0f;
	m_CubeCoord[3][1] = 0.5f;	m_TexCoord[3][1] = 1.0f;
}

void DHWnd::Render()
{
	if (!SetPixelFormat(m_pDC->m_hDC,
		ChoosePixelFormat(m_pDC->m_hDC, &m_Desc), &m_Desc))
		return;

	m_hRC = wglCreateContext(m_pDC->m_hDC);
	if (!m_hRC)
		return;

	if (!wglMakeCurrent(m_pDC->m_hDC, m_hRC))
		return;

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glVertexPointer(2, GL_FLOAT, 0, m_CubeCoord);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POLYGON, 0, 4);
	glFinish();

	SwapBuffers(m_pDC->m_hDC);
}