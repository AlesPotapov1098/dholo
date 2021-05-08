#include "pch.h"
#include "framework.h"
#include "DHApp.h"
#include "DHWnd.h"

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
	CDC * dc = GetDC();
	m_Transform->Init(*dc,dholo::gpgpu::DHOCLHost());
	m_Transform->GenerateTexture();
	m_Transform->Calculate();
	m_Transform->RenderScene();
}

void DHWnd::LoadTexture(const std::vector<CStringA>& path)
{
	if (!m_ImgLoader.empty())
		m_ImgLoader.clear();

	if (path.size() > 1)
		m_OperType = OperationType::DH_OPERATION_COMPLEX_CALCULATION;
	else if (path.size() == 1)
		m_OperType = OperationType::DH_OPERATION_NONE;

	int len = path.size();

	m_ImgLoader.resize(len);

	for (int i = 0; i < len; i++)
		m_ImgLoader[i].Load(path[i]);
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
		/// TODO: обработка ошибок
		return;

	m_OperType = OperationType::DH_OPERATION_NONE;

	m_ImgLoader.clear();
	m_ImgLoader.resize(1);
	m_ImgLoader[0].Load(imgPath);
}

void DHWnd::LoadImg(const std::vector<CStringA>& imgPaths)
{
	if (imgPaths.empty())
		/// TODO: обработка ошибок
		return;

	m_OperType = OperationType::DH_OPERATION_COMPLEX_CALCULATION;

	m_ImgLoader.clear();
	m_ImgLoader.resize(4);

	for (int i = 0; i < 4; i++)
		m_ImgLoader[i].Load(imgPaths[i]);
}

