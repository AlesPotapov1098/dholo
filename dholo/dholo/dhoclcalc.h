#pragma once

#include <afxwin.h>
#include <Windows.h>
#include <string>
#include <gl\GLU.h>
#include <gl\GL.h>
#include <fstream>

#include "idhcalculate.h"
#include "dhoclinit.h"

class DHOCLCalc : public IDHCalculate
{
public:
	DHOCLCalc() = default;
	~DHOCLCalc() = default;

	/// <summary>
	/// Инициализация полей класс m_Program и m_Kernel.
	/// Загрузка исходного кода программы и его компиляция.
	/// Создания ядра
	/// </summary>
	void virtual Init(const DHOCLHost& host, const std::string& progpath);

	/// <summary>
	/// Загрузка текстуры OpenGL.
	/// Инициализация объекта m_InOutMem
	/// </summary>
	void virtual LoadImg(const GLuint& texture);

	/// <summary>
	/// Передача ядру m_Kernel входных значений.
	/// Выполнение ядра с заданными параметрами
	/// </summary>
	void Calculate() override;

private:
	cl::Program m_Program;
	cl::Kernel m_Kernel;
	cl::Context m_Context;
	cl::Memory m_InOutMem;
	cl::CommandQueue m_CommandQueue;
};