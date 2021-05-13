#include "pch.h"
#include "DHAppExp.h"

namespace dholo
{
	namespace exp
	{
		DHAppExp::DHAppExp(const std::string& mess)
		{
			exception::exception(mess.c_str());
		}

		DHAppExp::DHAppExp(const CString& mess)
		{
			exception::exception(CStringA(mess).GetString());
		}

		char const* DHAppExp::what() const
		{
			AfxMessageBox(CStringW(what()), MB_OK);
			return exception::what();
		}
	}
}
