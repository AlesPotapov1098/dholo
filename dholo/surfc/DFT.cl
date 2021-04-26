// Умножение комплексных чисел типа float2 - вектор
// Тип возвращаемого значеия float2 - вектор
// complex_mul_float2_float2
// c1 = a + ib, c2 = c + id
// result = (ac-bd) + i(ad + bc)
float2 cmf2f2(float2 c1, float2 c2)
{
	return (float2)(c1.x * c2.x - c1.y * c2.y, c1.x * c2.y + c1.y * c2.x);
}

// Умножение комплексного чиса типа float2 - вектор и обычного типа float
// Тип возвращаемого значеия float2 - вектор
// complex_mul_float_float2
// r, c = a + ib
// result = ra + i(rb)
float2 cmff2(float r, float c)
{
	return (float2)(r * c.x, r * c.y);
}

// Дискретное преобразование Фурье
__kernel void DFT(
	__global float* x, // | Исходная последовательность
	__global float2* X  // | Преобразование Фурье
)
{
	int k = get_global_id(0);   // | k => X[k]
	int N = get_global_size(0); // | количество точек

	float W = 2 * M_PI_F / N;   // Множитель при экспоненте без n = 2pi*k / N
	float2 Wn;

	X[k] = (float2)(0.0f, 0.0f);

	for(int n = 0; n < N; n++)
	{
		Wn.x = cos(W * n);
		Wn.y = sin(W * n);
		X[k] += cmff2(x[n], Wn);
	}
}