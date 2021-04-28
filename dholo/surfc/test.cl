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
		// 0 ������� ������������������
		__global int * input,
		// 1 ���������� ��������� �� ������� ������������������
		int N, 
		// 2 ���������� ��������� � ������
		int Gn,
		// 3 ��������� ������ ��� ���������
		__local int * x,
		// 4 ���������� ���
		uint bits,
		// 5 �������� ������������������ (��������� ��������������)
		__global int * output)
{
	// ����� ������
	uint group_id = get_group_id(0);

	// ��������� ������ ��� ���������� � ��������� ������ �� ����������
	uint start_index = get_group_id(0) * Gn;

	for(uint i = 0; i < Gn; i++)
		x[i] = input[bit_reversed(i + start_index, bits)];

	for(int i = 0; i < Gn; i++)
		output[group_id * Gn + i] = x[i];
}
