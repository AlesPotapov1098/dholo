#include "pch.h"
#include "DHGPGPUExp.h"

namespace dholo
{
	namespace exp
	{
		DHGPGPUExp::DHGPGPUExp(cl_int error_code)
		{
			if (!m_ErrorMessage.LoadStringW((-1) * error_code))
				m_ErrorMessage = L"Unknown error";
		}

		DHGPGPUExp::DHGPGPUExp(cl_int error_code, const std::string &mess)
		{
			if (!m_ErrorMessage.LoadStringW((-1) * error_code))
				m_ErrorMessage = L"Unknown error";
			else
				m_ErrorMessage = m_ErrorMessage + L" " + CString(mess.c_str());
		}

		DHGPGPUExp::~DHGPGPUExp()
		{
		}
	}
}
