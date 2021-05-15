#include "pch.h"
#include "DHAppExp.h"

namespace dholo
{
	namespace exp
	{
		DHAppExp::DHAppExp(const std::string& mess)
		{
			m_ErrorMessage = CString(mess.c_str());
		}

		DHAppExp::DHAppExp(const CString& mess)
		{
			m_ErrorMessage = mess;
		}

		void DHAppExp::ShowError() const
		{
			AfxMessageBox(m_ErrorMessage, MB_OK);
		}
	}
}
