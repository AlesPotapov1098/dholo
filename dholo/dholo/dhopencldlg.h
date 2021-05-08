#pragma once
#include <afxwin.h>
#include <string>

#include "resource.h"

#include "dhoclinit.h"

namespace dholo
{
	namespace dlg 
	{
		class DHOpenCLDlg : public CDialogEx
		{
			DECLARE_DYNAMIC(DHOpenCLDlg)

		public:
			DHOpenCLDlg(CWnd* pParent = nullptr);   // стандартный конструктор
			virtual ~DHOpenCLDlg();

			gpgpu::DHOCLHost GetOpenCLHost() const;

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

			afx_msg BOOL OnInitDialog();

			afx_msg void OnOK();
			afx_msg void OnCancel();

			afx_msg void OnCbnSelchangeComboPlatfroms();
			afx_msg void OnCbnSelchangeComboDevice();

			void FillInComboPlatforms();
			void FillInPlatformPanel();

			void FillInComboDevice();
			void FillInDevicePanel();

			DECLARE_MESSAGE_MAP()

		private:
			gpgpu::DHOCLInit m_Init;
			gpgpu::DHOCLHost m_Host;
		};
	}
}
