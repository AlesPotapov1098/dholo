#pragma once

#include <exception>
#include <CL/cl.h>

namespace dholo
{
	namespace gpgpu
	{
		namespace err
		{
			class DHOCLErr : public std::exception
			{
			public:
				DHOCLErr(cl_int error_code);
				virtual ~DHOCLErr();
				
				virtual char const* ShowError() const;
			};
		}
	}
}