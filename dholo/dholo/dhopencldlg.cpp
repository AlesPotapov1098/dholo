#include "dholo.h"
#include "DHOpenCLDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(DHOpenCLDlg, CDialogEx)

DHOpenCLDlg::DHOpenCLDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CREATE_WINDOW, pParent)
{

}

DHOpenCLDlg::~DHOpenCLDlg()
{
}

void DHOpenCLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DHOpenCLDlg, CDialogEx)
END_MESSAGE_MAP()
