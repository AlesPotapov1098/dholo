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
		
	m_Transform = new dholo::gpgpu::DHGPGPUTransform;

	return 0;
}

void DHWnd::OnPaint() 
{
	m_pDC = BeginPaint(&m_Paint);
	m_Transform->Init(*m_pDC,dholo::gpgpu::DHOCLHost());
	m_Transform->GenerateTexture();
	m_Transform->Calculate();
	m_Transform->RenderScene();
	m_Transform->Release();
	EndPaint(&m_Paint);
}

void DHWnd::OnSize(UINT nType, int cx, int cy)
{
	GetWindowRect(&m_Rect);
	m_CRect.CopyRect(&m_Rect);
}

void DHWnd::LoadTexture(const std::vector<CStringA>& path)
{
	try {
		if (!m_ImgLoader.empty())
			m_ImgLoader.clear();

		if (path.empty())
			throw dholo::exp::DHAppExp("Not such filenames");

		int len = path.size();

		m_ImgLoader.resize(len);

		for (int i = 0; i < len; i++)
			m_ImgLoader[i].Load(path[i]);
	}
	catch (const dholo::exp::DHAppExp& ex)
	{
		ex.what();
	}
}

void DHWnd::GenerateTest()
{
	//dlg::DHOpenCLDlg dlg;
	//if (dlg.DoModal() != IDOK)
	//	return;
	//
	//m_Host = dlg.GetOpenCLHost();
	//m_OperType = OperationType::DH_OPERATION_TEST;
	//
	//m_Test.GenerateTextExmpl();

	Invalidate();
	UpdateWindow();
}

void DHWnd::LoadImg(const CStringA& imgPath)
{
	if (imgPath.IsEmpty())
		throw dholo::exp::DHAppExp("Invalid filename");

	m_ImgLoader.clear();
	m_ImgLoader.resize(1);
	m_ImgLoader[0].Load(imgPath);
}

void DHWnd::LoadImg(const std::vector<CStringA>& imgPaths)
{
	if (imgPaths.empty())
		throw dholo::exp::DHAppExp("Invalid filename");

	m_ImgLoader.clear();
	m_ImgLoader.resize(4);

	for (int i = 0; i < 4; i++)
		m_ImgLoader[i].Load(imgPaths[i]);
}

void DHWnd::GenSin()
{
	DHGENSinus dlg;
	if (dlg.DoModal() != IDOK)
		return;

	m_Transform = new dholo::gpgpu::DHGPGPUGenSinus(
		dlg.GetWidth(),
		dlg.GetHeight(),
		dlg.GetAmpl(),
		dlg.GetPhase(),
		dlg.GetT());

	Invalidate();
	UpdateWindow();
}

void DHWnd::PSITransform()
{
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
