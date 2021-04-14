#pragma once

#include <afxwin.h>
#include <Windows.h>
#include <string>
#include <gl\GLU.h>
#include <gl\GL.h>
#include <fstream>
#include <vector>

#include "idhcalculate.h"
#include "dhoclinit.h"

#include <CL\cl.h>
#include <CL\opencl.hpp>

class DHOCLCalc //: public IDHCalculate
{
public:
	DHOCLCalc() = default;
	~DHOCLCalc() = default;

	/// <summary>
	/// ������������� ����� ����� m_Program � m_Kernel.
	/// �������� ��������� ���� ��������� � ��� ����������.
	/// �������� ����
	/// </summary>
	void Init(const DHOCLHost& host, const std::string& progpath);

	/// <summary>
	/// �������� �������� OpenGL.
	/// ������������� ������� m_InOutMem
	/// </summary>
	void LoadImg(const GLuint& texture, int width, int height);

	/// <summary>
	/// �������� ���� m_Kernel ������� ��������.
	/// ���������� ���� � ��������� �����������
	/// </summary>
	void Calculate();

private:
	cl::Program m_Program;
	cl::Kernel m_Kernel;
	cl::Context m_Context;
	cl::Memory m_InOutMem;
	cl::CommandQueue m_CommandQueue;
	int m_width;
	int m_height;
};