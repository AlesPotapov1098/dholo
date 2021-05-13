#include "pch.h"
#include "DHGPGPUExp.h"

namespace dholo
{
	namespace exp
	{
		DHGPGPUExp::DHGPGPUExp(cl_int error_code)
		{
			CStringW errorMsg;
			if (errorMsg.LoadStringW(error_code))
				exception::exception(CStringA(errorMsg));
			else
				exception::exception("Unknown error");
		}

		DHGPGPUExp::DHGPGPUExp(cl_int error_code, const std::string &mess)
		{
			CStringW errorMsg;
			if (errorMsg.LoadStringW(error_code))
			{
				std::string error_msg = std::string(CStringA(errorMsg)) + " " + mess;
				exception::exception(error_msg.c_str());
			}
			else
				exception::exception("Unknown error");
		}

		DHGPGPUExp::~DHGPGPUExp()
		{
		}
	}
}
