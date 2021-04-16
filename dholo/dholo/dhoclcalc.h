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

class DHOCLCalc
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
	void LoadImg(const GLuint& texture);

	/// <summary>
	/// Установка глобальных параметров
	/// </summary>
	void SetGlobalPar(int global_x, int global_y);

	/// <summary>
	/// Установка локальных параметров
	/// </summary>
	void SetLocalPar(int local_x, int local_y);

	/// <summary>
	/// Передача ядру m_Kernel входных значений.
	/// Выполнение ядра с заданными параметрами
	/// </summary>
	void Calculate();

private:
	cl_program m_Program;
	cl_kernel m_Kernel;
	cl_context m_Context;
	cl_mem m_InOutMem;
	cl_command_queue m_CommandQueue;
	int m_GlobalX, m_GlobalY;
	int m_LocalX, m_LocalY;
};