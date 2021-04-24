int bitReverse(int i, int m)
{
	unsigned int j = i;

	j = (j & 0x55555555) << 1 | (j & 0xAAAAAAAA) >> 1;
	j = (j & 0x33333333) << 2 | (j & 0xCCCCCCCC) >> 2;
	j = (j & 0x0F0F0F0F) << 4 | (j & 0xF0F0F0F0) >> 4;
	j = (j & 0x00FF00FF) << 8 | (j & 0xFF00FF00) >> 8;
	j = (j & 0x0000FFFF) << 16 | (j & 0xFFFF0000) >> 16;

	j >>= (32 - m);

	return j;
}