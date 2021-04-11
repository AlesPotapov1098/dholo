#include "stdafx.h"
#include "dholo.h"
#include "dhwnd.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	LoadTexture();
	return CWnd::OnCreate(lpcst);
}

void DHWnd::InitOpenGL()
{
	m_Desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	m_Desc.nVersion = 1;
	m_Desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	m_Desc.iPixelType = PFD_TYPE_RGBA;
	m_Desc.cColorBits = 64;
	m_Desc.cDepthBits = 32;
	m_Desc.iLayerType = PFD_MAIN_PLANE;
}

void DHWnd::LoadTexture()
{
	int nrChannels;
	data = stbi_load("res\\images.jpg", &WIDTH, &HEIGHT, &nrChannels, 0);
	if (!data)
	{
		AfxMessageBox(L"Ошибка при загрузке текстуры");
		stbi_image_free(data);
	}
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
	
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_OpenGLTexture);
	glBindTexture(GL_TEXTURE_2D, m_OpenGLTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-0.95f, -0.95f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-0.95f, 0.95f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(0.95f, 0.95f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(0.95f, -0.95f);
	glEnd();

	SwapBuffers(m_pDC->m_hDC);
}