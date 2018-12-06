__kernel void Encrypt(__global char* bytes, __global char* s)
{
	int index = get_global_id(0);
	 
	for (int i = 0; i < 8; i++)
	{
		// узнать 1 или 0 i бит в s[index]
		// поставить бит в последний бит соответствующего байта в bytes
		if (s[index] & (1 << i))
			bytes[index * 8 + i] |= 1; 
		else
			bytes[index * 8 + i] &= 254;
	}
}