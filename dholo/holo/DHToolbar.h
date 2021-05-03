#pragma once

namespace dholo
{
	namespace toolbar
	{
		class DHToolBar : public CMFCToolBar
		{
			virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
			{
				CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
			}

			virtual BOOL AllowShowOnList() const { return FALSE; }
		};
	}
}
