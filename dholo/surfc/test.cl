__kernel void testKernel(__global float * input, int m, __global float * output)
{
	int id = get_global_id(0);
	int N = get_global_size(0);
	float resutl = 0.0f;

	for(int i = 0; i < N; i++)
	{
		resutl += input[i] * id;
	}

	output[id] = resutl;
}
