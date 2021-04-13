#pragma once

/// <summary>
/// ��������� ��� ��������� �����������
/// </summary>
class IDHCalculate
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
	/// ����� ��� ��������� �����������
	/// </summary>
	void virtual Calculate() = 0;
};