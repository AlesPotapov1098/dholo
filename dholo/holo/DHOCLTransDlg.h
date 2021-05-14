#pragma once

#include "DHOCLInit.h"
#include "DHGPGPUPSITransform.h"

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

		void FillInComboPlatforms();
		void FillInPlatformPanel();

		void FillInComboDevice();
		void FillInDevicePanel();

		gpgpu::PSIStruct GetPSISettings() const;

		DECLARE_MESSAGE_MAP()

	private:
		gpgpu::DHOCLInit m_Init;
		gpgpu::DHOCLHost m_Host;

		gpgpu::PSIStruct m_PSISettings;
	};
}
