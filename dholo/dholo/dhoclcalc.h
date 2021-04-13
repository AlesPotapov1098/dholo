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
	/// Инициализация полей класс m_Program и m_Kernel.
	/// Загрузка исходного кода программы и его компиляция.
	/// Создания ядра
	/// </summary>
	void virtual Init(const DHOCLHost& host, const CStringA& progpath);

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
	cl::Memory m_InOutMem;
	cl::CommandQueue m_CommandQueue;
};