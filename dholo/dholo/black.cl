#define OPENCL EXTENSION cl_khr_gl_sharing : enable

__kernel void testKernelWrite(__read_write image2d_t resultTexture)
{	
	int2 imgCoords = (int2)(get_global_id(0), get_global_id(1));
	
	uint4 imgVal = (uint4)(255, 0, 0, 255);

	write_imageui(resultTexture, imgCoords, imgVal);
}