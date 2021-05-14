#pragma once

#include <iostream>

#include "resource.h"

template<typename T = int>
inline T GetNumberFromEdit(CEdit* edit)
{
	CString strNumber;
	edit->GetWindowTextW(strNumber);
	return static_cast<T>(std::atoi(static_cast<CStringA>(strNumber)));
}
