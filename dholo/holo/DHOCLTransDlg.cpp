#include "pch.h"
#include "DHOCLTransDlg.h"
#include "afxdialogex.h"
#include "resource.h"

namespace dholo
{
	IMPLEMENT_DYNAMIC(DHOCLTransDlg, CDialogEx)

		DHOCLTransDlg::DHOCLTransDlg(CWnd* pParent /*=nullptr*/)
		: CDialogEx(IDD_OCL_TRANSFORM, pParent)
	{

	}

	DHOCLTransDlg::~DHOCLTransDlg()
	{
	}

	void DHOCLTransDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	}


	BEGIN_MESSAGE_MAP(DHOCLTransDlg, CDialogEx)
		ON_CBN_SELCHANGE(IDC_COMBO_PLATFORM_NAME,
			&DHOCLTransDlg::OnCbnSelchangeComboPlatformName)

		ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_NAME,
			&DHOCLTransDlg::OnCbnSelchangeComboDeviceName)
	END_MESSAGE_MAP()


	BOOL DHOCLTransDlg::OnInitDialog()
	{
		if (!CDialogEx::OnInitDialog())
			return FALSE;

		//if (!m_Init.Init())
		//	return FALSE;

		FillInComboPlatforms();
		FillInPlatformPanel();

		FillInComboDevice();
		FillInDevicePanel();

		return TRUE;
	}

	void DHOCLTransDlg::OnOK()
	{
		int nPlatform = \
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME))->GetCurSel();
		int nDevice = \
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE_NAME))->GetCurSel();

		auto hard = m_Init.GetHardware(nPlatform);
		m_Host.SetPlatform(hard.GetPlatform());
		m_Host.SetDevice(hard.GetDevice(nDevice));

		CDialogEx::OnOK();
	}

	void DHOCLTransDlg::OnCancel()
	{
		CDialogEx::OnCancel();
	}


	void DHOCLTransDlg::OnCbnSelchangeComboPlatformName()
	{
		// TODO: добавьте свой код обработчика уведомлений
	}


	void DHOCLTransDlg::OnCbnSelchangeComboDeviceName()
	{
		// TODO: добавьте свой код обработчика уведомлений
	}

}
