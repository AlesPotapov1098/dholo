#include "stdafx.h"
#include "dhoclcalc.h"

void DHOCLCalc::Init(const DHOCLHost& host, const std::string& progpath)
{
	if (progpath.empty())
		return;

	m_Context = cl::Context(host.GetDevice());

	std::ifstream file(progpath, std::ios_base::binary);
	std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
	cl::Program::Sources src(1, cl::string(code.c_str()));

	m_Program = cl::Program(m_Context, src);
	cl_int err = m_Program.build(host.GetDevice());
	if (err != CL_SUCCESS)
		AfxMessageBox(L"Ошибка компиляции");

	m_Kernel = cl::Kernel(m_Program, "paint_it_black");
}

void DHOCLCalc::LoadImg(const GLuint& texture)
{
}

void DHOCLCalc::Calculate()
{
}
