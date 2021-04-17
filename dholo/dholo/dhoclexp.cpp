#include "stdafx.h"
#include "dhoclexp.h"

namespace dholo
{
	namespace exp
	{
		DHOCLException::DHOCLException(const char* msg)
			: exception(msg)
		{
		}

		DHOCLException::DHOCLException(int code)
		{
		}

		DHOCLException::~DHOCLException()
		{
		}
	}
}