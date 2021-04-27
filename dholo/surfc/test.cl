__kernel void testKernel(
		__global float * input,
		__global float * output)
{
	int group = get_group_id(0);
	int id = get_local_id(0);
	int index = group * 2 + id;
	int start = index * 5;
	__local float arr[5];
	
	for(int i = 0; i < 5; i++)
	{
		arr[i] = input[start + i];
	}

	float res = 0.0f;

	for(int i = 0; i < 5; i++)
	{
		res += arr[i];
	}

	output[index] = res;
}
