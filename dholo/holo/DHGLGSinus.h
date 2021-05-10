#pragma once

class DHGENSinus : public CDialogEx
{
	DECLARE_DYNAMIC(DHGENSinus)

public:
	DHGENSinus(CWnd* pParent = nullptr);
	virtual ~DHGENSinus();
	virtual void DoDataExchange(CDataExchange* pDX);

	float GetAmpl() const;
	float GetPhase() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetChannels() const;
	int GetT() const;

protected:
	afx_msg BOOL OnInitDialog();

	afx_msg void OnBnClickedCheckPhaseRandom();

	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()
	
	template<typename T = int>
	T GetNumberFromEdit(CEdit*);

private:
	float m_Ampl;
	float m_Phase;
	int m_Width;
	int m_Height;
	int m_Channels;
	int m_T;
};

template<typename T>
inline T DHGENSinus::GetNumberFromEdit(CEdit* edit)
{
	CString strNumber;
	edit->GetWindowTextW(strNumber);
	return static_cast<T>(std::atoi(static_cast<CStringA>(strNumber)));
}
