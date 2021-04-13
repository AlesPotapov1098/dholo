#pragma once

/// <summary>
/// Интерфейс для обработки изображения
/// </summary>
class IDHCalculate
{
public:
	/// <summary>
	/// Методя для инициализации
	/// </summary>
	void virtual Init() = 0;

	/// <summary>
	/// Метод для загрузки изображения
	/// </summary>
	void virtual LoadImg() = 0;

	/// <summary>
	/// Метод для обработки изображения
	/// </summary>
	void virtual Calculate() = 0;
};