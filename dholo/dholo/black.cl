__kernel void paint_it_black(__read_write img2d_t img)
{
	const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_LINEAR;

	int2 coord = (int2)(get_global_id(0), get_global_id(1));

	uint4 bgra = read_imageui(img, smp, coord); 
	bgra.x = 0;
	bgra.y = 0;
	bgra.z = 0;

	write_imageuiimg, coord, bgra);
}