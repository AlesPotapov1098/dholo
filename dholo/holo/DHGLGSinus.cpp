#include "pch.h"
#include "DHGLGSinus.h"
#include "afxdialogex.h"
#include "resource.h"

#include <CL/cl.h>
#include <gl/GL.h>

IMPLEMENT_DYNAMIC(DHGENSinus, CDialogEx)

DHGENSinus::DHGENSinus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_GEN_SIN, pParent)
{
	m_Ampl = 1.0f;
	m_Phase = 0.0f;
	m_Width = 100;
	m_Height = 100;
	m_Channels = 3;
	m_T = 1;
}

DHGENSinus::~DHGENSinus()
{
}

void DHGENSinus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DHGENSinus, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_PHASE_RANDOM, &DHGENSinus::OnBnClickedCheckPhaseRandom)
END_MESSAGE_MAP()

BOOL DHGENSinus::OnInitDialog()
{
	if (!CDialogEx::OnInitDialog())
		return FALSE;

	auto comboPhase = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PHASES));

	comboPhase->AddString(L"0");
	comboPhase->AddString(L"pi/2");
	comboPhase->AddString(L"pi");
	comboPhase->AddString(L"3pi/2");
	
	comboPhase->SetCurSel(0);

	auto colorModel = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_COLOR_MODEL));
	
	colorModel->AddString(L"RGB");
	colorModel->AddString(L"RGBA");

	colorModel->SetCurSel(0);

	auto editWidth = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_GEN_SIN_W));
	editWidth->SetWindowTextW(L"1024");

	auto editHeight = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_GEN_SIN_H));
	editHeight->SetWindowTextW(L"1024");

	auto editAmpl = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_GEN_SIN_AMPL));
	editAmpl->SetWindowTextW(L"1");

	auto editNumPeriods = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_GEN_SIN_NUM_PERIODS));
	editNumPeriods->SetWindowTextW(L"10");

	return TRUE;
}

void DHGENSinus::OnBnClickedCheckPhaseRandom()
{
	auto checkBoxMyPhase = static_cast<CButton*>(this->GetDlgItem(IDC_CHECK_PHASE_RANDOM));
	int checked = checkBoxMyPhase->GetCheck();

	auto editPhase = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_SIN_PHASE));
	auto comboPhase = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PHASES));

	editPhase->EnableWindow(checked == BST_CHECKED ? TRUE : FALSE);
	comboPhase->EnableWindow(checked == BST_CHECKED ? FALSE : TRUE);
}

void DHGENSinus::OnOK()
{
	auto width = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_GEN_SIN_W));
	m_Width = GetNumberFromEdit(width);

	auto height = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_GEN_SIN_H));
	m_Height = GetNumberFromEdit(height);

	auto ampl = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_GEN_SIN_AMPL));
	m_Ampl = GetNumberFromEdit<float>(ampl);

	auto checkBoxMyPhase = static_cast<CButton*>(this->GetDlgItem(IDC_CHECK_PHASE_RANDOM));
	int checked = checkBoxMyPhase->GetCheck();
	if (checked == BST_CHECKED)
	{
		auto phase = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_SIN_PHASE));
		m_Phase = GetNumberFromEdit<float>(phase);
	}
	else
	{
		auto phase = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_PHASES));
		int numerator = phase->GetCurSel();
		m_Phase = numerator * CL_M_PI_2;
	}

	auto colorModel = static_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_COLOR_MODEL));
	int index = colorModel->GetCurSel();

	m_Channels = index == 0 ? GL_RGB : GL_RGBA;

	auto periods = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_GEN_SIN_NUM_PERIODS));
	m_T = GetNumberFromEdit(periods);

	CDialogEx::OnOK();
}

float DHGENSinus::GetAmpl() const { return m_Ampl; }

float DHGENSinus::GetPhase() const { return m_Phase; }

int DHGENSinus::GetWidth() const { return m_Width; }

int DHGENSinus::GetHeight() const { return m_Height; }

int DHGENSinus::GetChannels() const { return m_Channels; }

int DHGENSinus::GetT() const { return m_T; }
