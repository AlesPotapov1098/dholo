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
		// 0 Входная последовательность
		__global int * input,
		// 1 Количество элементов во входной последовательности
		int N, 
		// 2 Количество элементов в группе
		int Gn,
		// 3 Локальный массив для обработки
		__local int * x,
		// 4 Количество бит
		uint bits,
		// 5 Выходная последовательность (результат перобразования)
		__global int * output)
{
	// Номер группы
	uint group_id = get_group_id(0);

	// Начальный индекс для считывания в локальную память из глобальной
	uint start_index = get_group_id(0) * Gn;

	for(uint i = 0; i < Gn; i++)
		x[i] = input[bit_reversed(i + start_index, bits)];

	for(int i = 0; i < Gn; i++)
		output[group_id * Gn + i] = x[i];
}
