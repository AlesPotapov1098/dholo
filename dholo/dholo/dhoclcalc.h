#pragma once

#include <afxwin.h>
#include <Windows.h>

#include <gl\GLU.h>
#include <gl\GL.h>

#include "idhcalculate.h"
#include "dhoclinit.h"

class DHOCLCalc : public IDHCalculate
{
public:
	DHOCLCalc() = default;
	~DHOCLCalc() = default;

	/// <summary>
	/// ������������� ����� ����� m_Program � m_Kernel.
	/// �������� ��������� ���� ��������� � ��� ����������.
	/// �������� ����
	/// </summary>
	void virtual Init(const DHOCLHost& host, const CStringA& progpath);

	/// <summary>
	/// �������� �������� OpenGL.
	/// ������������� ������� m_InOutMem
	/// </summary>
	void virtual LoadImg(const GLuint& texture);

	/// <summary>
	/// �������� ���� m_Kernel ������� ��������.
	/// ���������� ���� � ��������� �����������
	/// </summary>
	void Calculate() override;

private:
	cl::Program m_Program;
	cl::Kernel m_Kernel;
	cl::Memory m_InOutMem;
	cl::CommandQueue m_CommandQueue;
};