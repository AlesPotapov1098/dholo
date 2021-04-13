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
	virtual void Init() {};

	/// <summary>
	/// Метод для создания изображения
	/// </summary>
	virtual void LoadImg() {};

	/// <summary>
	/// Методя для отрисовки изображения
	/// </summary>
	virtual void Draw() {};
};