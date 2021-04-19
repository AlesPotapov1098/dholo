#include "stdafx.h"

#include "dholo.h"
#include "DHOpenCLDlg.h"
#include "afxdialogex.h"

namespace dholo
{
	namespace dlg {

		IMPLEMENT_DYNAMIC(DHOpenCLDlg, CDialogEx)
		
		BEGIN_MESSAGE_MAP(DHOpenCLDlg, CDialogEx)
			ON_CBN_SELCHANGE(IDC_COMBO_PLATFROMS, 
				&DHOpenCLDlg::OnCbnSelchangeComboPlatfroms)
		
			ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, 
				&DHOpenCLDlg::OnCbnSelchangeComboDevice)
		END_MESSAGE_MAP()
		
		DHOpenCLDlg::DHOpenCLDlg(CWnd* pParent)
			: CDialogEx(IDD_DIALOG_CREATE_WINDOW, pParent)
		{
		
		}
		
		DHOpenCLDlg::~DHOpenCLDlg()
		{
		}
		
		ocl::DHOCLHost DHOpenCLDlg::GetOpenCLHost() const
		{
			return m_Host;
		}
		
		void DHOpenCLDlg::DoDataExchange(CDataExchange* pDX)
		{
			CDialogEx::DoDataExchange(pDX);
		}
		
		BOOL DHOpenCLDlg::OnInitDialog()
		{
			if (!CDialog::OnInitDialog())
				return FALSE;
		
			if (!m_Init.Init())
				return FALSE;
		
			FillInComboPlatforms();
			FillInPlatformPanel();
		
			FillInComboDevice();
			FillInDevicePanel();
		
			return TRUE;
		}
		
		void DHOpenCLDlg::OnOK()
		{
			int nPlatform = \
				static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFROMS))->GetCurSel();
			int nDevice = \
				static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE))->GetCurSel();
		
			auto hard = m_Init.GetHardware(nPlatform);
			m_Host.SetPlatform(hard.GetPlatform());
			m_Host.SetDevice(hard.GetDevice(nDevice));
		
			CDialog::OnOK();
		}
		
		void DHOpenCLDlg::OnCancel()
		{
			CDialog::OnCancel();
		}
		
		void DHOpenCLDlg::OnCbnSelchangeComboPlatfroms()
		{
			FillInPlatformPanel();
			FillInDevicePanel();
		}
		
		void DHOpenCLDlg::OnCbnSelchangeComboDevice()
		{
			FillInDevicePanel();
		}
		
		void DHOpenCLDlg::FillInComboPlatforms()
		{
			auto comboBox = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFROMS));
			ASSERT(comboBox != nullptr);
		
			if (comboBox->GetCount() != 0)
				comboBox->ResetContent();
		
			int countHard = m_Init.GetSize();
			ocl::DHOCLHard hard;
		
			for (int i = 0; i < countHard; i++)
			{
				hard = m_Init.GetHardware(i);
				int countDevs = hard.GetCountDevices();
		
				for (int j = 0; j < countDevs; j++)
					comboBox->AddString(m_Init.GetPlatformName(hard.GetPlatform()).c_str());
			}
		
			comboBox->SetCurSel(0);
		}
		
		void DHOpenCLDlg::FillInPlatformPanel()
		{
			auto comboBox = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFROMS));
		
			int CurSell = comboBox->GetCurSel();
			ocl::DHOCLHard hard = m_Init.GetHardware(CurSell);
		
			auto editPlatformVendor =
				static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_PLATFORM_VENDOR));
			editPlatformVendor->SetWindowTextW(m_Init.GetPlatformVendor(hard.GetPlatform()).c_str());
		
			auto editPlatformVersion =
				static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_PLATFORM_VERSION));
			editPlatformVersion->SetWindowTextW(m_Init.GetPlatformVersion(hard.GetPlatform()).c_str());
		}
		
		void DHOpenCLDlg::FillInComboDevice()
		{
			auto comboBox =
				static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFROMS));
		
			int curSell = comboBox->GetCurSel();
			ocl::DHOCLHard hard = m_Init.GetHardware(curSell);
		
			auto comboDevices =
				static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE));
			int countDevs = hard.GetCountDevices();
		
			for (int i = 0; i < countDevs; i++)
				comboDevices->AddString(m_Init.GetDeviceName(hard[i]).c_str());
		
			comboDevices->SetCurSel(0);
		}
		
		void DHOpenCLDlg::FillInDevicePanel()
		{
			auto comboBox =
				static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PLATFROMS));
		
			int curSell = comboBox->GetCurSel();
			ocl::DHOCLHard hard = m_Init.GetHardware(curSell);
		
			auto comboDevice =
				static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_DEVICE));
		
			int curSellComboDevices = comboDevice->GetCurSel();
		
			auto editDeviceVendor =
				static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_VENDOR));
			editDeviceVendor->SetWindowTextW(m_Init.GetDeviceVendor(hard[curSellComboDevices]).c_str());
		
			auto editDeviceVersion =
				static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_VERSION));
			editDeviceVersion->SetWindowTextW(m_Init.GetDeviceVersion(hard[curSellComboDevices]).c_str());

			auto editDeviceType =
				static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEVICE_TYPE));
			editDeviceType->SetWindowTextW(m_Init.GetDeviceType(hard[curSellComboDevices]).c_str());
		}
	}
}