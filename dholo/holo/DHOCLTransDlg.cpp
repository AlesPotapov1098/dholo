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
		FillInPlatformPanel();
		FillInDevicePanel();
	}


	void DHOCLTransDlg::OnCbnSelchangeComboDeviceName()
	{
		FillInDevicePanel();
	}

	void DHOCLTransDlg::FillInComboPlatforms()
	{
		auto comboBox = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));
		ASSERT(comboBox != nullptr);

		if (comboBox->GetCount() != 0)
			comboBox->ResetContent();

		int countHard = m_Init.GetSize();
		ocl::DHOCLHard hard;
		ocl::DHOCLInfo info;

		for (int i = 0; i < countHard; i++)
		{
			hard = m_Init.GetHardware(i);
			int countDevs = hard.GetCountDevices();

			for (int j = 0; j < countDevs; j++)
				comboBox->AddString(info.GetPlatformName(hard.GetPlatform()).c_str());
		}

		comboBox->SetCurSel(0);
	}

	void DHOCLTransDlg::FillInPlatformPanel()
	{
		auto comboBox = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int CurSell = comboBox->GetCurSel();
		ocl::DHOCLHard hard = m_Init.GetHardware(CurSell);
		ocl::DHOCLInfo info;

		auto editPlatformVendor =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_PLATFORM_VENDOR));
		editPlatformVendor->SetWindowTextW(info.GetPlatformVendor(hard.GetPlatform()).c_str());

		auto editPlatformVersion =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_PLATFORM_VERSION));
		editPlatformVersion->SetWindowTextW(info.GetPlatformVersion(hard.GetPlatform()).c_str());
	}

	void DHOCLTransDlg::FillInComboDevice()
	{
		auto comboBox =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int curSell = comboBox->GetCurSel();
		ocl::DHOCLHard hard = m_Init.GetHardware(curSell);
		ocl::DHOCLInfo info;

		auto comboDevices =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE_NAME));
		int countDevs = hard.GetCountDevices();

		for (int i = 0; i < countDevs; i++)
			comboDevices->AddString(info.GetDeviceName(hard[i]).c_str());

		comboDevices->SetCurSel(0);
	}

	void DHOCLTransDlg::FillInDevicePanel()
	{
		auto comboBox =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int curSell = comboBox->GetCurSel();
		ocl::DHOCLHard hard = m_Init.GetHardware(curSell);
		ocl::DHOCLInfo info;

		auto comboDevice =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE_NAME));

		int curSellComboDevices = comboDevice->GetCurSel();

		auto editDeviceVendor =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_VENDOR));
		editDeviceVendor->SetWindowTextW(info.GetDeviceVendor(hard[curSellComboDevices]).c_str());

		auto editDeviceVersion =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_VERSION));
		editDeviceVersion->SetWindowTextW(info.GetDeviceVersion(hard[curSellComboDevices]).c_str());

		auto editDeviceType =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_TYPE));
		editDeviceType->SetWindowTextW(info.GetDeviceType(hard[curSellComboDevices]).c_str());
	}

}
