#include "pch.h"
#include "DHImgList.h"


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

namespace dholo
{
	namespace img
	{
		DHImgList::DHImgList() noexcept
		{
		}

		DHImgList::~DHImgList()
		{
		}

		BEGIN_MESSAGE_MAP(DHImgList, CListCtrl)
		END_MESSAGE_MAP()

		BOOL DHImgList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
		{
			BOOL bRes = CListCtrl::OnNotify(wParam, lParam, pResult);

			NMHDR* pNMHDR = (NMHDR*)lParam;
			ASSERT(pNMHDR != nullptr);

			if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
			{
				GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
			}

			return bRes;
		}
	}
}