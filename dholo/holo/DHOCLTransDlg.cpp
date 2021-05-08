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

		auto& hard = m_Init.GetHardware(nPlatform);
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

		for (int i = 0; i < countHard; i++)
		{
			int countDevs = m_Init.GetHardware(i).GetCountDevices();

			for (int j = 0; j < countDevs; j++)
				comboBox->AddString(ocl::DHOCLInfo::GetPlatformName(m_Init.GetHardware(i).
					GetPlatform()).c_str());
		}

		comboBox->SetCurSel(0);
	}

	void DHOCLTransDlg::FillInPlatformPanel()
	{
		auto comboBox = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int CurSell = comboBox->GetCurSel();
		ocl::DHOCLHard hard = m_Init.GetHardware(CurSell);

		auto editPlatformVendor =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_PLATFORM_VENDOR));
		editPlatformVendor->SetWindowTextW(
			ocl::DHOCLInfo::GetPlatformVendor(hard.GetPlatform()).c_str());

		auto editPlatformVersion =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_PLATFORM_VERSION));
		editPlatformVersion->SetWindowTextW(
			ocl::DHOCLInfo::GetPlatformVersion(hard.GetPlatform()).c_str());
	}

	void DHOCLTransDlg::FillInComboDevice()
	{
		auto comboBox =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int curSell = comboBox->GetCurSel();
		ocl::DHOCLHard hard = m_Init.GetHardware(curSell);

		auto comboDevices =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE_NAME));
		int countDevs = hard.GetCountDevices();

		for (int i = 0; i < countDevs; i++)
			comboDevices->AddString(
				ocl::DHOCLInfo::GetDeviceName(hard[i]).c_str());

		comboDevices->SetCurSel(0);
	}

	void DHOCLTransDlg::FillInDevicePanel()
	{
		auto comboBox =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int curSell = comboBox->GetCurSel();
		ocl::DHOCLHard hard = m_Init.GetHardware(curSell);

		auto comboDevice =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE_NAME));

		int curSellComboDevices = comboDevice->GetCurSel();

		auto editDeviceVendor =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_VENDOR));
		editDeviceVendor->SetWindowTextW(
			ocl::DHOCLInfo::GetDeviceVendor(hard[curSellComboDevices]).c_str());

		auto editDeviceVersion =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_VERSION));
		editDeviceVersion->SetWindowTextW(
			ocl::DHOCLInfo::GetDeviceVersion(hard[curSellComboDevices]).c_str());

		auto editDeviceType =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_TYPE));
		editDeviceType->SetWindowTextW(
			ocl::DHOCLInfo::GetDeviceType(hard[curSellComboDevices]).c_str());
	}

}
