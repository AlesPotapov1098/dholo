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
	/// Инициализация полей класс m_Program и m_Kernel.
	/// Загрузка исходного кода программы и его компиляция.
	/// Создания ядра
	/// </summary>
	void Init(const DHOCLHost& host, const std::string& progpath);

	/// <summary>
	/// Загрузка текстуры OpenGL.
	/// Инициализация объекта m_InOutMem
	/// </summary>
	void LoadImg(const GLuint& texture, int width, int height);

	/// <summary>
	/// Передача ядру m_Kernel входных значений.
	/// Выполнение ядра с заданными параметрами
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