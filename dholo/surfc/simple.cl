#define OPENCL EXTENSION cl_khr_gl_sharing : enable

__kernel void testKernelWrite(
	__write_only image2d_t img1, 
	__write_only image2d_t img2, 
	__write_only image2d_t img3, 
	__write_only image2d_t img4)
{	
	int2 imgCoords = (int2)(get_global_id(0), get_global_id(1));
	
	float4 imgVal = (float4)(1.0f, 0.0f, 0.0f, 1.0f);

	write_imagef(img1, imgCoords, imgVal);

	imgVal = (float4)(0.0f, 1.0f, 0.0f, 1.0f);

	write_imagef(img2, imgCoords, imgVal);

	imgVal = (float4)(0.0f, 0.0f, 1.0f, 1.0f);

	write_imagef(img3, imgCoords, imgVal);

	imgVal = (float4)(0.0f, 0.0f, 0.0f, 1.0f);

	write_imagef(img4, imgCoords, imgVal);
}
