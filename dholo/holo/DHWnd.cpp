#include "pch.h"
#include "framework.h"
#include "DHApp.h"
#include "DHWnd.h"
#include "DHGLGSinus.h"

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
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL DHWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_OWNDC, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

int DHWnd::OnCreate(LPCREATESTRUCT lpcst)
{
	if (CWnd::OnCreate(lpcst) != 0)
		return -1;
		
	ZeroMemory(&m_Desc, sizeof(m_Desc));

	m_Desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	m_Desc.nVersion = 1;
	m_Desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	m_Desc.iPixelType = PFD_TYPE_RGBA;
	m_Desc.cColorBits = 32;
	m_Desc.cDepthBits = 24;
	m_Desc.iLayerType = PFD_MAIN_PLANE;

	m_Transform = new dholo::gpgpu::DHGPGPUTransform;
	
	return 0;
}

void DHWnd::OnPaint() 
{
	m_pDC = BeginPaint(&m_Paint);
	
	try 
	{
		InitHandles();

		m_Transform->Init(m_Host, m_pDC->m_hDC, m_hRC);
		m_Transform->Calculate(m_GlobalSizeX, m_GlobalSizeY, m_LocalSizeX, m_LocalSizeY);
		m_Transform->Release();

		Render();
	}
	catch (const dholo::exp::DHGPGPUExp &ex)
	{
		ex.ShowError();
	}

	EndPaint(&m_Paint);
}

void DHWnd::OnSize(UINT nType, int cx, int cy)
{
	GetWindowRect(&m_Rect);
	m_CRect.CopyRect(&m_Rect);
}

void DHWnd::LoadImg(const CStringA& imgPath)
{
	if (imgPath.IsEmpty())
		throw dholo::exp::DHAppExp("Invalid filename");

	m_ImgLoader.Load(imgPath);

	m_GlobalSizeX = m_ImgLoader.GetWidth();
	m_GlobalSizeY = m_ImgLoader.GetHeight();

	m_LocalSizeX = 1;
	m_LocalSizeY = 1;

	m_Transform = new dholo::gpgpu::DHGPGPUTransform(&m_RenderTargetTexture, m_ImgLoader);

	Invalidate();
	UpdateWindow();
}

void DHWnd::GenSin()
{
	DHGENSinus dlg;
	if (dlg.DoModal() != IDOK)
		return;

	m_Transform = new dholo::gpgpu::DHGPGPUGenSinus(
		&m_RenderTargetTexture,
		dlg.GetWidth(),
		dlg.GetHeight(),
		dlg.GetAmpl(),
		dlg.GetPhase(),
		dlg.GetT());

	Invalidate();
	UpdateWindow();
}

void DHWnd::PSITransform(const dholo::gpgpu::PSIStruct &psi, const dholo::gpgpu::DHOCLHost &host)
{
	m_Transform = new dholo::gpgpu::DHGPGPUPSITransform(&m_RenderTargetTexture, psi, &m_ImgLoader);
	m_Host = host;

	Invalidate();
	UpdateWindow();
}

ImageFile DHWnd::OnSaveImg()
{
	CString filter = L"PNG File (*.png) || BMP File (*.bmp) || JPEG File (*.jpg)";
	CFileDialog openFileDlg(TRUE, L"png", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result != IDOK)
		return { false };

	GUID guid = Gdiplus::ImageFormatBMP;

	CImage image;
	image.Create(m_CRect.Width(), m_CRect.Height(), 32);
	BitBlt(image.GetDC(), 0, 0, m_CRect.Width(), m_CRect.Height(), GetWindowDC()->m_hDC, 0, 0, SRCCOPY);
	image.Save(openFileDlg.GetPathName());
	image.ReleaseDC();
	image.Destroy();

	return { true, openFileDlg.GetPathName(), openFileDlg.GetFileExt(), openFileDlg.GetFileName() };
}

void DHWnd::OnGetAmplitude()
{
	Invalidate();
	UpdateWindow();
}

void DHWnd::OnGetPhase()
{
	Invalidate();
	UpdateWindow();
}

void DHWnd::OnGetRe()
{
	Invalidate();
	UpdateWindow();
}

void DHWnd::OnGetIm()
{
	Invalidate();
	UpdateWindow();
}

void DHWnd::InitHandles()
{
	if (m_pDC == nullptr)
		return;

	if (!SetPixelFormat(m_pDC->m_hDC,
		ChoosePixelFormat(m_pDC->m_hDC, &m_Desc),
		&m_Desc))
		return;

	m_hRC = wglCreateContext(m_pDC->m_hDC);
	if (!m_hRC)
		return;

	if (!wglMakeCurrent(m_pDC->m_hDC, m_hRC))
		return;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void DHWnd::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_RenderTargetTexture);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);
	glEnd();



	SwapBuffers(m_pDC->m_hDC);
}
