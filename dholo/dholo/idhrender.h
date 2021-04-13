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
	virtual void Init() = 0;

	/// <summary>
	/// Метод для создания изображения
	/// </summary>
	virtual void LoadImg() = 0;

	/// <summary>
	/// Методя для отрисовки изображения
	/// </summary>
	virtual void Draw() = 0;
};