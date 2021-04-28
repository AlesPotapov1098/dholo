// ������� �������������� ������� �����.
uint bit_reversed(uint x, uint bits) {
	uint y = 0;
	for (int i = 0; i < bits; i++) {
		y <<= 1;
		y |= x & 1;
		x >>= 1;
	}
	y &= ((1 << bits) - 1);
	return y;
}


__kernel void testKernel(
	// 0 - ������� ������������������.
		__global int * input,
	// 1 - ���������� ��������� �� ������� ������������������.
		int N,
	// 2 - ���������� ��������� � ������.
		int Gn,
	// 3 - ��������� ������ ��� ���������.
		__local int * arr,
	// 4 - ���������� ���.
		uint bits,
	// 5 - ���������� ������� ��� �������, ������� ����� ���� ���������� ����� �������
		uint nlevels,
	// 6 - �������� ������������������ (��������� ��������������).
		__global int * output)
{
	// ����� ������.
		uint group_id = get_group_id(0);

	// ��������� ������ ��� ���������� � ��������� ������ �� ����������.
		uint start_index = get_group_id(0) * Gn;

	// ��������� ��������� ������ ���������� �� �����������.
	// �������� ���������� ������� ������������� ������� �� ����������� �������
	// � ��������� ����� ������������ �������� ������� ��� ���������� ������� ���� ��������� ������.
		for(uint i = 0; i < Gn; i++)
			arr[i] = input[bit_reversed(i + start_index, bits)];

	// ��� ������ ���������� �������, ������� �� �������� ������
		int step = 1;
	
	// ��������
		int offset = 1;
	
	// ���������� ������
		int cycles = 1;
	
	// ��������� �������������� ����� ��� �������� �� ���������� �������.
	// 1 - ������������ �� �������. ������������ ������� ������� �� �������� �������� nlevels.
		for(int l = 1; l <= nlevels; l++)
		{
			// ������ ���, � ������� ����� ���� �� ���������� �������.
				step <<= 1;

			// ������������ �������� ������.
			// ���������� ��������� ������� �� �������� step, 
			// ������� ������ ��� ������������� � 2 ����.
				for(int i = 0; i < Gn; i += step)
					// ������� ������������������.
					// ��������� ���������� ��������, ������� �� ����� � �������� � ���������� �������.
						for(int j = 0; j < cycles; j++)
						{
							int a = arr[i + j];
							int b = arr[i + j + offset];

							arr[i + j] = a + b;
							arr[i + j + offset] = a - b;
						}

			// �������� �������� �������� � ���������� ������ ������
				offset <<= 1;
				cycles <<= 1;
		}

	// ��������� ���������� �������������� ������
		for(int i = 0; i < Gn; i++)
			output[group_id * Gn + i] = arr[i];
}
