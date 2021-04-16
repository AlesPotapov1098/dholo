#pragma once

namespace dholo
{
	namespace img
	{
		class DHImgList : public CListCtrl
		{
			// Создание
		public:
			DHImgList() noexcept;
			virtual ~DHImgList();

			// Переопределение
		protected:
			virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

			DECLARE_MESSAGE_MAP()
		};
	}
}