#pragma once

#include "DHOCLInit.h"
#include "DHGPGPUPSITransform.h"
#include "DHHelp.h"

namespace dholo {
	class DHOCLTransDlg : public CDialogEx
	{
		DECLARE_DYNAMIC(DHOCLTransDlg)

	public:
		DHOCLTransDlg(CWnd* pParent = nullptr);
		virtual ~DHOCLTransDlg();

		virtual void DoDataExchange(CDataExchange* pDX);

		afx_msg BOOL OnInitDialog();

		afx_msg void OnOK();
		afx_msg void OnCancel();

		afx_msg void OnCbnSelchangeComboPlatformName();
		afx_msg void OnCbnSelchangeComboDeviceName();

		afx_msg void OnBnClickedCheckPhaseRandom();
		afx_msg void OnBnClickedCheckPhaseRandom2();
		afx_msg void OnBnClickedCheckPhaseRandom3();
		afx_msg void OnBnClickedCheckPhaseRandom4();

		void FillInComboPlatforms();
		void FillInPlatformPanel();

		void FillInComboDevice();
		void FillInDevicePanel();

		void FillInPSIPanel();
		void FillInComboPhase(int code);

		float GetPhase1() const;
		float GetPhase2() const;
		float GetPhase3() const;
		float GetPhase4() const;

		dholo::gpgpu::DHOCLHost GetHost() const;

		void CheckRandomPahesBox(int check, int combo, int edit);

		DECLARE_MESSAGE_MAP()

	private:

		template<typename T = int>
		inline T GetNumberFromControl(int checkBoxID, int editID, int comboID)
		{
			auto checkBoxMyPhase = static_cast<CButton*>(this->GetDlgItem(checkBoxID));
			int checked = checkBoxMyPhase->GetCheck();

			if (checked == BST_CHECKED)
			{
				auto phase = static_cast<CEdit*>(this->GetDlgItem(editID));
				return GetNumberFromEdit<T>(phase);
			}
			else
			{
				auto phase = static_cast<CComboBox*>(this->GetDlgItem(comboID));
				int numerator = phase->GetCurSel();
				return numerator * CL_M_PI_2;
			}
		}

	private:
		gpgpu::DHOCLInit m_Init;
		gpgpu::DHOCLHost m_Host;

		float m_Phase1;
		float m_Phase2;
		float m_Phase3;
		float m_Phase4;
		float m_B;
	};
}
