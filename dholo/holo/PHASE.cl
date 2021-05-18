__kernel PHASE( __write_only image2d_t InOut)
{
	 const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE |
         CLK_ADDRESS_CLAMP |
         CLK_FILTER_NEAREST;

    int2 coord = (int2)(get_global_id(0), get_global_id(1));

    float4 val = read_imagef(InOut, smp, coord);

	float phase = atan2(val.x, val.y);

	phase += M_PI;
	phase /= 2 * M_PI;

	write_imagef(InOut, coord, (float4)(phase, phase, phase, 1.0f));
}