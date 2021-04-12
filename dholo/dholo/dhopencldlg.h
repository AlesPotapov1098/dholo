#pragma once
#include <afxwin.h>
#include <string>

#include "resource.h"

#include "dhoclinit.h"

class DHOpenCLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DHOpenCLDlg)

public:
	DHOpenCLDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~DHOpenCLDlg();

	DHOCLHost GetOpenCLHost() const;

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

	void FillListBox(CListBox*, const std::wstring&);

	DECLARE_MESSAGE_MAP()

private:
	DHOCLInit m_Init;
	DHOCLHost m_Host;
};
