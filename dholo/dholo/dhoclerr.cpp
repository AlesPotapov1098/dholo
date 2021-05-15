#include "stdafx.h"

#include "dhoclerr.h"

dholo::gpgpu::err::DHOCLErr::DHOCLErr(cl_int error_code)
{
	CStringW errorMsg;
	if(errorMsg.LoadStringW(error_code))
		exception::exception(CStringA(errorMsg));
	else
		exception::exception("Unknown error");
	
}

dholo::gpgpu::err::DHOCLErr::~DHOCLErr()
{
}

char const* dholo::gpgpu::err::DHOCLErr::ShowError() const
{
	AfxMessageBox(CStringW(exception::ShowError()), MB_OK);
	return exception::ShowError();
}
