#pragma once

#include <exception>

namespace dholo
{
	namespace exp
	{
		class DHOCLException : public std::exception
		{
		public:
			DHOCLException() = default;
			DHOCLException(const char* msg);
			DHOCLException(int code);
			virtual ~DHOCLException();
		};
	}
}