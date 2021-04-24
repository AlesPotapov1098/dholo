// ��������� ����������� ����� ���� float2 - ������
// ��� ������������� ������� float2 - ������
// complex_mul_float2_float2
// c1 = a + ib, c2 = c + id
// result = (ac-bd) + i(ad + bc)
float2 cmf2f2(float2 c1, float2 c2)
{
	return (float2)(c1.x * c2.x - c1.y * c2.y, c1.x * c2.y + c1.y * c2.x);
}

// ��������� ������������ ���� ���� float2 - ������ � �������� ���� float
// ��� ������������� ������� float2 - ������
// complex_mul_float_float2
// r, c = a + ib
// result = ra + i(rb)
float2 cmff2(float r, float c)
{
	return (float2)(r * c.x, r * c.y);
}

// ���������� �������������� �����
__kernel void DFT(
	__global float* x, // | �������� ������������������
	__global float2* X  // | �������������� �����
)
{
	int k = get_global_id(0);   // | k => X[k]
	int N = get_global_size(0); // | ���������� �����

	float W = 2 * M_PI_F / N;   // ��������� ��� ���������� ��� n = 2pi*k / N
	float2 Wn;

	X[k] = (float2)(0.0f, 0.0f);

	for(int n = 0; n < N; n++)
	{
		Wn.x = cos(W * n);
		Wn.y = sin(W * n);
		X[k] += cmff2(x[n], Wn);
	}
}