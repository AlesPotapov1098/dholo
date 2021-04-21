#define OPENCL EXTENSION cl_khr_gl_sharing : enable

// скалярное произведение векторов
float vecMul(
	float4 v1, 
	float4 v2)
{
   return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w * v2.w;
}

float2 complexFromPolarCoordinates(
	float magnitude, 
	float phase)
{
    return (float2)(magnitude * cos(phase), magnitude * sin(phase));
}

__kernel void psi4Kernel(
	__read_only image2d_t img0, //0
	__read_only image2d_t img1, //1
	__read_only image2d_t img2, //2
	__read_only image2d_t img3, //3
	float4 k_sin,				//4
	float4 k_cos,				//5
	float znmt_abs,				//6
	__write_only image2d_t output) //7
{
    const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | //Natural coordinates
         CLK_ADDRESS_CLAMP | //Clamp to zeros
         CLK_FILTER_NEAREST; //Don't interpolate

    int2 coord = (int2)(get_global_id(0), get_global_id(1));

    float4 val0 = read_imagef(img0, smp, coord);
    float4 val1 = read_imagef(img1, smp, coord);
    float4 val2 = read_imagef(img2, smp, coord);
    float4 val3 = read_imagef(img3, smp, coord);

    float4 i_sdv = (float4)(val0.x, val1.x, val2.x, val3.x);

    float IC = vecMul(i_sdv, k_cos);
    float IS = vecMul(i_sdv, k_sin);

    float a = atan2(IC, IS);
    float am = sqrt(IS * IS + IC * IC) / znmt_abs;

    float2 result = complexFromPolarCoordinates(am, a);
    
	a += M_PI;
	a /= 2 * M_PI;

	write_imagef(output, coord, (float4)(a, a, a, 0.0f));
}