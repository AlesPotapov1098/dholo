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
		try
		{
			if (!CDialogEx::OnInitDialog())
				return FALSE;

			if (!m_Init.Init())
				return FALSE;

			FillInComboPlatforms();
			FillInPlatformPanel();

			FillInComboDevice();
			FillInDevicePanel();
		}
		catch (const dholo::exp::DHGPGPUExp& ex)
		{
			ex.what();
		}

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
		try
		{
			FillInPlatformPanel();
			FillInDevicePanel();
		}
		catch (const dholo::exp::DHGPGPUExp& ex)
		{
			ex.what();
		}
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
				comboBox->AddString(gpgpu::DHOCLInfo::GetPlatformName(m_Init.GetHardware(i).
					GetPlatform()).c_str());
		}

		comboBox->SetCurSel(0);
	}

	void DHOCLTransDlg::FillInPlatformPanel()
	{
		auto comboBox = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int CurSell = comboBox->GetCurSel();
		gpgpu::DHOCLHard hard = m_Init.GetHardware(CurSell);

		auto editPlatformVendor =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_PLATFORM_VENDOR));
		editPlatformVendor->SetWindowTextW(
			gpgpu::DHOCLInfo::GetPlatformVendor(hard.GetPlatform()).c_str());

		auto editPlatformVersion =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_PLATFORM_VERSION));
		editPlatformVersion->SetWindowTextW(
			gpgpu::DHOCLInfo::GetPlatformVersion(hard.GetPlatform()).c_str());
	}

	void DHOCLTransDlg::FillInComboDevice()
	{
		auto comboBox =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int curSell = comboBox->GetCurSel();
		gpgpu::DHOCLHard hard = m_Init.GetHardware(curSell);

		auto comboDevices =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE_NAME));
		int countDevs = hard.GetCountDevices();

		for (int i = 0; i < countDevs; i++)
			comboDevices->AddString(
				gpgpu::DHOCLInfo::GetDeviceName(hard[i]).c_str());

		comboDevices->SetCurSel(0);
	}

	void DHOCLTransDlg::FillInDevicePanel()
	{
		auto comboBox =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME));

		int curSell = comboBox->GetCurSel();
		gpgpu::DHOCLHard hard = m_Init.GetHardware(curSell);

		auto comboDevice =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE_NAME));

		int curSellComboDevices = comboDevice->GetCurSel();

		auto editDeviceVendor =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_VENDOR));
		editDeviceVendor->SetWindowTextW(
			gpgpu::DHOCLInfo::GetDeviceVendor(hard[curSellComboDevices]).c_str());

		auto editDeviceVersion =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_VERSION));
		editDeviceVersion->SetWindowTextW(
			gpgpu::DHOCLInfo::GetDeviceVersion(hard[curSellComboDevices]).c_str());

		auto editDeviceType =
			static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_TYPE));
		editDeviceType->SetWindowTextW(
			gpgpu::DHOCLInfo::GetDeviceType(hard[curSellComboDevices]).c_str());
	}

	void DHOCLTransDlg::FillInPSIPanel()
	{
		FillInComboPhase(IDC_COMBO_PHASES1);
		FillInComboPhase(IDC_COMBO_PHASES2);
		FillInComboPhase(IDC_COMBO_PHASES3);
		FillInComboPhase(IDC_COMBO_PHASES4);
	}

	void DHOCLTransDlg::FillInComboPhase(int code)
	{
		auto comboPhase = static_cast<CComboBox*>(this->GetDlgItem(code));

		comboPhase->AddString(L"0");
		comboPhase->AddString(L"pi/2");
		comboPhase->AddString(L"pi");
		comboPhase->AddString(L"3pi/2");
	}

	gpgpu::PSIStruct DHOCLTransDlg::GetPSISettings() const
	{
		return m_PSISettings;
	}

}
