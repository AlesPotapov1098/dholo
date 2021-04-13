#pragma once

/// <summary>
/// ��������� ��� ��������� �����������
/// </summary>
class IDHRender
{
public:
	/// <summary>
	/// ������ ��� �������������
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// ����� ��� �������� �����������
	/// </summary>
	virtual void LoadImg() = 0;

	/// <summary>
	/// ������ ��� ��������� �����������
	/// </summary>
	virtual void Draw() = 0;
};