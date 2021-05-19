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
		ON_BN_CLICKED(IDC_CHECK_PHASE_RANDOM, &DHOCLTransDlg::OnBnClickedCheckPhaseRandom)
		ON_BN_CLICKED(IDC_CHECK_PHASE_RANDOM2, &DHOCLTransDlg::OnBnClickedCheckPhaseRandom2)
		ON_BN_CLICKED(IDC_CHECK_PHASE_RANDOM3, &DHOCLTransDlg::OnBnClickedCheckPhaseRandom3)
		ON_BN_CLICKED(IDC_CHECK_PHASE_RANDOM4, &DHOCLTransDlg::OnBnClickedCheckPhaseRandom4)
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

			FillInPSIPanel();
		}
		catch (const dholo::exp::DHGPGPUExp& ex)
		{
			ex.ShowError();
		}
		catch (const dholo::exp::DHAppExp& ex)
		{
			ex.ShowError();
		}

		return TRUE;
	}

	void DHOCLTransDlg::OnOK()
	{
		int nPlatform =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFORM_NAME))->GetCurSel();
		int nDevice =
			static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE_NAME))->GetCurSel();

		auto& hard = m_Init.GetHardware(nPlatform);
		m_Host.SetPlatform(hard.GetPlatform());
		m_Host.SetDevice(hard.GetDevice(nDevice));

		m_Phase1 = GetNumberFromControl<float>(IDC_CHECK_PHASE_RANDOM1, IDC_EDIT_SIN_PHASE1, IDC_COMBO_PHASES1);
		m_Phase2 = GetNumberFromControl<float>(IDC_CHECK_PHASE_RANDOM2, IDC_EDIT_SIN_PHASE2, IDC_COMBO_PHASES2);
		m_Phase3 = GetNumberFromControl<float>(IDC_CHECK_PHASE_RANDOM3, IDC_EDIT_SIN_PHASE3, IDC_COMBO_PHASES3);
		m_Phase4 = GetNumberFromControl<float>(IDC_CHECK_PHASE_RANDOM4, IDC_EDIT_SIN_PHASE4, IDC_COMBO_PHASES4);

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
			ex.ShowError();
		}
		catch (const dholo::exp::DHAppExp& ex)
		{
			ex.ShowError();
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

		FillEdit(IDC_EDIT_PLATFORM_VENDOR, gpgpu::DHOCLInfo::GetPlatformVendor(hard.GetPlatform()));
		FillEdit(IDC_EDIT_PLATFORM_VERSION, gpgpu::DHOCLInfo::GetPlatformVersion(hard.GetPlatform()));
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

		FillEdit(IDC_EDIT_DEVICE_VENDOR, gpgpu::DHOCLInfo::GetDeviceVendor(hard[curSellComboDevices]));
		FillEdit(IDC_EDIT_DEVICE_VERSION, gpgpu::DHOCLInfo::GetDeviceVersion(hard[curSellComboDevices]));
		FillEdit(IDC_EDIT_DEVICE_TYPE, gpgpu::DHOCLInfo::GetDeviceType(hard[curSellComboDevices]));
		FillEdit(IDC_EDIT_DEVICE_FREQ, gpgpu::DHOCLInfo::GetDeviceFrequency(hard[curSellComboDevices]));
		FillEdit(IDC_EDIT_GLOBAL_MEM, gpgpu::DHOCLInfo::GetDeviceGlobalSize(hard[curSellComboDevices]));
		FillEdit(IDC_EDIT_LOCAL_MEM, gpgpu::DHOCLInfo::GetDeviceLocalSize(hard[curSellComboDevices]));
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

	void DHOCLTransDlg::FillEdit(int id, const std::wstring & str)
	{
		auto editByID =
			static_cast<CEdit*>(this->GetDlgItem(id));

		if (editByID == nullptr)
			throw dholo::exp::DHAppExp("Проблемы с edit box");

		editByID->SetWindowTextW(str.c_str());
	}

	float DHOCLTransDlg::GetPhase1() const
	{
		return m_Phase1;
	}

	float DHOCLTransDlg::GetPhase2() const
	{
		return m_Phase2;
	}

	float DHOCLTransDlg::GetPhase3() const
	{
		return m_Phase3;
	}

	float DHOCLTransDlg::GetPhase4() const
	{
		return m_Phase4;
	}

	dholo::gpgpu::DHOCLHost DHOCLTransDlg::GetHost() const
	{
		return m_Host;
	}

	void DHOCLTransDlg::CheckRandomPahesBox(int check, int combo, int edit)
	{
		auto checkBoxMyPhase = static_cast<CButton*>(this->GetDlgItem(check));
		int checked = checkBoxMyPhase->GetCheck();

		auto comboPhase = static_cast<CComboBox*>(this->GetDlgItem(combo));
		auto editPhase = static_cast<CEdit*>(this->GetDlgItem(edit));

		editPhase->EnableWindow(checked == BST_CHECKED ? TRUE : FALSE);
		comboPhase->EnableWindow(checked == BST_CHECKED ? FALSE : TRUE);
	}

	void DHOCLTransDlg::OnBnClickedCheckPhaseRandom()
	{
		CheckRandomPahesBox(
			IDC_CHECK_PHASE_RANDOM1,
			IDC_COMBO_PHASES1,
			IDC_EDIT_SIN_PHASE1);
	}


	void DHOCLTransDlg::OnBnClickedCheckPhaseRandom2()
	{
		CheckRandomPahesBox(
			IDC_CHECK_PHASE_RANDOM2,
			IDC_COMBO_PHASES2,
			IDC_EDIT_SIN_PHASE2);
	}


	void DHOCLTransDlg::OnBnClickedCheckPhaseRandom3()
	{
		CheckRandomPahesBox(
			IDC_CHECK_PHASE_RANDOM3,
			IDC_COMBO_PHASES3,
			IDC_EDIT_SIN_PHASE3);
	}


	void DHOCLTransDlg::OnBnClickedCheckPhaseRandom4()
	{
		CheckRandomPahesBox(
			IDC_CHECK_PHASE_RANDOM4,
			IDC_COMBO_PHASES4,
			IDC_EDIT_SIN_PHASE4);
	}
}
