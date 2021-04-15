// TODO: Add OpenCL kernel code here.
#define OPENCL EXTENSION cl_khr_gl_sharing : enable

__kernel void SimpleKernel(
	__write_only image2d_t img1, 
	__write_only image2d_t img2,
	__write_only image2d_t img3,
	__write_only image2d_t img4,
	__write_only image2d_t outImg)
	{
		int2 imgCoords = (int2)(get_global_id(0), get_global_id(1);

	}