#pragma once

/// <summary>
/// Интерфейс для отрисовки изображения
/// </summary>
class IDHRender
{
public:
	/// <summary>
	/// Методя для инициализации
	/// </summary>
	void virtual Init() = 0;

	/// <summary>
	/// Метод для создания изображения
	/// </summary>
	void virtual LoadImg() = 0;

	/// <summary>
	/// Методя для отрисовки изображения
	/// </summary>
	void virtual Draw() = 0;
};