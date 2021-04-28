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
		// ¬ходна€ последовательность
		__global int * input,

		int N, 
		__global int * output)
{

}
