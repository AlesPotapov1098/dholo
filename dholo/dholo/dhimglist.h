#pragma once

class DHImgList : public CListCtrl
{
	// ��������
public:
	DHImgList() noexcept;
	virtual ~DHImgList();

	// ���������������
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};