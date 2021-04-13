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
	void virtual Init() = 0;

	/// <summary>
	/// ����� ��� �������� �����������
	/// </summary>
	void virtual LoadImg() = 0;

	/// <summary>
	/// ������ ��� ��������� �����������
	/// </summary>
	void virtual Draw() = 0;
};