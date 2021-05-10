#pragma once
#include "dhwnd.h"
#include "dhimgexpr.h"
#include "dhtoolbar.h"

namespace dholo
{
	namespace holo
	{
		class DHMainFrm : public CFrameWndEx
		{

		public:
			DHMainFrm();
		protected:
			DECLARE_DYNAMIC(DHMainFrm)

		public:
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
			virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
			virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
			void SelectImage(const std::vector<CStringA>& imgPath);

			/// <summary>
			/// Вывод одной картинки в главное окно
			/// </summary>
			void LoadImg(const CStringA& imgPath);
			void LoadImg(const std::vector<CStringA>& imgPaths);

		public:
			virtual ~DHMainFrm();
#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

		protected:  // встроенные члены панели элементов управления
			CMFCMenuBar       m_wndMenuBar;
			toolbar::DHToolBar       m_FunctionalToolBar;
			toolbar::DHToolBar m_TransformToolBar;
			CMFCStatusBar     m_wndStatusBar;
			CMFCToolBarImages m_UserImages;
			DHWnd    m_wndView;

			img::DHImgExpr m_imgExpr;

		protected:
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			afx_msg void OnSetFocus(CWnd *pOldWnd);
			afx_msg void OnViewCustomize();
			afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
			afx_msg void OnApplicationLook(UINT id);
			afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

			afx_msg void OnLoadImg();
			afx_msg void OnSaveImage();
			afx_msg void OnTest();

			DECLARE_MESSAGE_MAP()

		};
	}
}

