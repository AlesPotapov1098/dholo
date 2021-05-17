#include "pch.h"
#include "framework.h"
#include "DHApp.h"

#include "DHMainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(DHMainFrm, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(DHMainFrm, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &DHMainFrm::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &DHMainFrm::OnToolbarCreateNew)
	ON_COMMAND(ID_TOOLBAR_GEN_SIN, &DHMainFrm::OnGenSin)
	ON_COMMAND(ID_SAVE_IMG, &DHMainFrm::OnSaveImg)
	ON_COMMAND(ID_SAVE_AND_ADD_IMG, &DHMainFrm::OnSaveAndAddImg)

	// Обработка сообщений от контекстного меню
	ON_COMMAND(ID_MENU_ADD_IMAGE, &DHMainFrm::OnAddImage)
	ON_COMMAND(ID_MENU_DELETE_IMAGE, &DHMainFrm::OnDeleteImage)
	ON_COMMAND(ID_MENU_LOAD_GP, &DHMainFrm::OnLoadIntoGp)
	ON_COMMAND(ID_MENU_PSI_TRANS, &DHMainFrm::OnPSITransform)
	
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

DHMainFrm::DHMainFrm() noexcept
{
	// TODO: добавьте код инициализации члена
}

DHMainFrm::~DHMainFrm()
{
}

int DHMainFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Не удалось создать строку меню\n");
		return -1;      // не удается создать
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// предотвращение фокусировки строки меню на активации 
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	// создать представление для размещения рабочей области рамки
	if (!m_targetWnd.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("Не удалось создать окно представлений\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Не удалось создать панель инструментов\n");
		return -1;      // не удалось создать
	}

	if (!m_TransformToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_TransformToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Не удалось создать панель инструментов\n");
		return -1;      // не удалось создать
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Разрешить операции с пользовательскими панелями инструментов:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Не удалось создать строку состояния\n");
		return -1;      // не удалось создать
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: удалите эти пять строк, если панель инструментов и строка меню не должны быть закрепляемыми
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_TransformToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_TransformToolBar);

	CString strImgPanel;
	bNameValid = strImgPanel.LoadStringW(IDS_IMAGE_LIST);
	ASSERT(bNameValid);

	if (!m_imgList.Create(strImgPanel, this, CRect(0, 0, 200, 200), TRUE, ID_IMAGE_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Не удалось создать окно \"Представление файлов\"\n");
		return FALSE;
	}

	m_imgList.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_imgList);

	// включить режим работы закрепляемых окон стилей Visual Studio 2005
	CDockingManager::SetDockingMode(DT_SMART);
	// включить режим работы автоматического скрытия закрепляемых окон стилей Visual Studio 2005
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// установите наглядный диспетчер, предназначенный для рисования всех элементов интерфейса пользователя
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	// Включить функцию замены меню панелей инструментов и закрепляемых окон 
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// включить быструю (Alt+перетаскивание) настройку панелей инструментов
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// загрузить изображения пользовательских панелей инструментов
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// включить персонализацию меню (последние использованные команды)
	// TODO: определите свои основные команды так, чтобы каждое раскрывающееся меню содержало по крайней мере одну основную команду.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL DHMainFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// Диагностика DHMainFrm

#ifdef _DEBUG
void DHMainFrm::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void DHMainFrm::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

void DHMainFrm::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// передача фокуса окну представления
	m_targetWnd.SetFocus();
}

BOOL DHMainFrm::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// разрешить ошибки в представлении при выполнении команды
	if (m_targetWnd.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// в противном случае выполняется обработка по умолчанию
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void DHMainFrm::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* сканировать меню*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT DHMainFrm::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void DHMainFrm::OnGenSin()
{
	m_targetWnd.GenSin();
}

void DHMainFrm::OnPSITransform()
{
	try {
		if (m_imgList.GetSelectedImageCount() != 4)
			throw dholo::exp::DHAppExp("Необходимо выбрать 4 изображения");

		dholo::DHOCLTransDlg dlg;
		HRESULT resDlg = dlg.DoModal();
		if (resDlg != IDOK)
			return;

		dholo::gpgpu::PSIStruct psi;

		psi.m_Phases[0] = dlg.GetPhase1();
		psi.m_Phases[1] = dlg.GetPhase2();
		psi.m_Phases[2] = dlg.GetPhase3();
		psi.m_Phases[3] = dlg.GetPhase4();

		m_imgList.SelectImages(&psi);

		m_targetWnd.PSITransform(psi, dlg.GetHost());
	}
	catch (const dholo::exp::DHAppExp &ex)
	{
		ex.ShowError();
	}
}

void DHMainFrm::OnContextMenu(CWnd* pWnd, CPoint point)
{
	
}

void DHMainFrm::OnDeleteImage()
{
	try
	{
		m_imgList.DeleteSelectedImg();
	}
	catch (const dholo::exp::DHAppExp& ex)
	{
		ex.ShowError();
	}
}

void DHMainFrm::OnAddImage()
{
	try
	{
		CFileDialog dlg(TRUE, NULL, L"*.jpg; *.png", OFN_ALLOWMULTISELECT);
	
		if (dlg.DoModal() != IDOK)
			return;
	
		POSITION ps = dlg.GetStartPosition();
		while (ps)
		{
			CString fileName = dlg.GetFileName();
			CString fileExt = dlg.GetFileExt();
			CString pathName = dlg.GetNextPathName(ps);
			m_imgList.AddImage(pathName, fileName, fileExt);
		}
	}
	catch (const dholo::exp::DHAppExp& ex)
	{
		ex.ShowError();
	}
}

void DHMainFrm::OnLoadIntoGp()
{
	int d = 0;
}

void DHMainFrm::OnMenu(CPoint point)
{
	if (point != CPoint(-1, -1))
	{
		CPoint ptTree = point;
		m_targetWnd.ScreenToClient(&ptTree);
	}

	m_imgList.SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU2, point.x, point.y, this, TRUE);
}

BOOL DHMainFrm::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// базовый класс не работает

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// включить кнопку настройки для всех пользовательских панелей инструментов
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void DHMainFrm::SelectImage(const std::vector<CStringA>& imgPath)
{
	try
	{
		if (!m_targetWnd)
			return;

		m_targetWnd.LoadTexture(imgPath);
		m_targetWnd.Invalidate();
		m_targetWnd.UpdateWindow();
	}
	catch (const dholo::exp::DHAppExp& ex)
	{
		ex.ShowError();
	}
}
void DHMainFrm::LoadImg(const CStringA& imgPath)
{
	try 
	{
		if (!m_targetWnd)
			/// TODO : обработка ошибок
			return;

		m_targetWnd.LoadImg(imgPath);
		m_targetWnd.Invalidate();
		m_targetWnd.UpdateWindow();
	}
	catch (const dholo::exp::DHAppExp& ex)
	{
		ex.ShowError();
	}
}
void DHMainFrm::LoadImg(const std::vector<CStringA>& imgPaths)
{
	try
	{
		if (!m_targetWnd)
			throw dholo::exp::DHAppExp("Not created a m_targetWnd");

		m_targetWnd.LoadImg(imgPaths);
		m_targetWnd.Invalidate();
		m_targetWnd.UpdateWindow();
	}
	catch (const dholo::exp::DHAppExp& ex)
	{
		ex.ShowError();
	}
}

void DHMainFrm::OnSaveImg()
{
	ImageFile imgFile = m_targetWnd.OnSaveImg();
	if (!imgFile.isOK)
		return;
}

void DHMainFrm::OnSaveAndAddImg()
{
	try
	{
		ImageFile imgFile = m_targetWnd.OnSaveImg();
		if (!imgFile.isOK)
			return;

		m_imgList.AddImage(imgFile.path, imgFile.name, imgFile.ext);
	}
	catch (const dholo::exp::DHAppExp& ex)
	{
		ex.ShowError();
	}
}
