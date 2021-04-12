#pragma once

// Диалоговое окно DHOpenCLDlg
#include <afxwin.h>

class DHOpenCLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DHOpenCLDlg)

public:
	DHOpenCLDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~DHOpenCLDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATE_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
