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
	virtual void Init() {};

	/// <summary>
	/// ����� ��� �������� �����������
	/// </summary>
	virtual void LoadImg() {};

	/// <summary>
	/// ������ ��� ��������� �����������
	/// </summary>
	virtual void Draw() {};
};