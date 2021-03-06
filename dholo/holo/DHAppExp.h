#pragma once

#include <exception>
#include <string>

namespace dholo
{
	namespace exp
	{
		class DHAppExp : public std::exception
		{
		public:
			DHAppExp() = default;
			DHAppExp(const std::string& mess);
			DHAppExp(const CString& mess);
			~DHAppExp() = default;

			void ShowError() const;

		protected:
			CString m_ErrorMessage;
		};
	}
}
