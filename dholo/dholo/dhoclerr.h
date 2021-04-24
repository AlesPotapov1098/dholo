#pragma once

#include <exception>
#include <CL/cl.h>

namespace dholo
{
	namespace ocl
	{
		namespace err
		{
			class DHOCLErr : public std::exception
			{
			public:
				DHOCLErr(cl_int error_code);
				virtual ~DHOCLErr();
				
				virtual char const* what() const;
			};
		}
	}
}