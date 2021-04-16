#define OPENCL EXTENSION cl_khr_gl_sharing : enable

__kernel void testKernelWrite(__read_write image2d_t resultTexture)
{	
	int2 imgCoords = (int2)(get_global_id(0), get_global_id(1));
	
	float4 imgVal = (float4)(1.0f, 0.0f, 0.0f, 1.0f);

	write_imagef(resultTexture, imgCoords, imgVal);
}