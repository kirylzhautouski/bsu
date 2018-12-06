__kernel void Decrypt(__global char* bytes, __global char* s)
{
	int index = get_global_id(0);

	for (int i = 0; i < 8; i++)
	{ 
		if (bytes[index * 8 + i] & 1)
			s[index] |= 1 << i;
	}
}