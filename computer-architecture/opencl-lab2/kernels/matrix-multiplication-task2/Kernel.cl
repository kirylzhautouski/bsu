__kernel void ScalarMultiply(__global double* m1, __global double* m2, __global double* m3, int n, int m, int l)
{
	int i = get_global_id(0);
	int j = get_global_id(1);

	double sum = 0;
	int m1Base = i * m;
	int m2Base = j * m;

	for (int k = 0; k < m; ++k)
	{
		sum += m1[m1Base + k] * m2[m2Base + k];
	}

	m3[i * l + j] = sum;
}

__kernel void Vector8Multiply(__global double* m1, __global double* m2, __global double* m3, int n, int m, int l)
{
	int i = get_global_id(0);
	int j = get_global_id(1);

	double8 sum = (double8)(0, 0, 0, 0, 0, 0, 0, 0);
	int m1Base = i * m;
	int m2Base = j * m;
	int bound = m / 8;

	for (int k = 0; k < bound; ++k)
	{
		double8 v1 = (double8)(m1[m1Base + 8 * k], m1[m1Base + 1 + 8 * k], m1[m1Base + 2 + 8 * k], m1[m1Base + 3 + 8 * k], m1[m1Base + 4 + 8 * k], m1[m1Base + 5 + 8 * k], m1[m1Base + 6 + 8 * k], m1[m1Base + 7 + 8 * k]);
		double8 v2 = (double8)(m2[m2Base + 8 * k], m2[m2Base + 1 + 8 * k], m2[m2Base + 2 + 8 * k], m2[m2Base + 3 + 8 * k], m2[m2Base + 4 + 8 * k], m2[m2Base + 5 + 8 * k], m2[m2Base + 6 + 8 * k], m2[m2Base + 7 + 8 * k]);
		sum += v1 * v2;
	}

	m3[i * l + j] = sum.s0 + sum.s1 + sum.s2 + sum.s3 + sum.s4 + sum.s5 + sum.s6 + sum.s7;
}

__kernel void VectorMultiply(__global double* m1, __global double* m2, __global double* m3, int n, int m, int l)
{
	int i = get_global_id(0);
	int j = get_global_id(1);

	double16 sum16 = (double16)(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	int m1Base = i * m;
	int m2Base = j * m;
	int bound16 = m / 16;

	for (int k = 0; k < bound16; ++k)
	{
		double16 v1 = (double16)(m1[m1Base + 16 * k], m1[m1Base + 1 + 16 * k], m1[m1Base + 2 + 16 * k], m1[m1Base + 3 + 16 * k], m1[m1Base + 4 + 16 * k], m1[m1Base + 5 + 16 * k], m1[m1Base + 6 + 16 * k], m1[m1Base + 7 + 16 * k], m1[m1Base + 8 + 16 * k], m1[m1Base + 9 + 16 * k], m1[m1Base + 10 + 16 * k], m1[m1Base + 11 + 16 * k], m1[m1Base + 12 + 16 * k], m1[m1Base + 13 + 16 * k], m1[m1Base + 14 + 16 * k], m1[m1Base + 15 + 16 * k]);
		double16 v2 = (double16)(m2[m2Base + 16 * k], m2[m2Base + 1 + 16 * k], m2[m2Base + 2 + 16 * k], m2[m2Base + 3 + 16 * k], m2[m2Base + 4 + 16 * k], m2[m2Base + 5 + 16 * k], m2[m2Base + 6 + 16 * k], m2[m2Base + 7 + 16 * k], m2[m2Base + 8 + 16 * k], m2[m2Base + 9 + 16 * k], m2[m2Base + 10 + 16 * k], m2[m2Base + 11 + 16 * k], m2[m2Base + 12 + 16 * k], m2[m2Base + 13 + 16 * k], m2[m2Base + 14 + 16 * k], m2[m2Base + 15 + 16 * k]);
		sum16 += v1 * v2;
	}

	double4 sum4 = (double4)(0, 0, 0, 0);
	m1Base += bound16 * 16;
	m2Base += bound16 * 16;
	int bound4 = (m - bound16 * 16) / 4;

	for (int k = 0; k < bound4; ++k)
	{
		double4 v1 = (double4)(m1[m1Base + 4 * k], m1[m1Base + 1 + 4 * k], m1[m1Base + 2 + 4 * k], m1[m1Base + 3 + 4 * k]);
		double4 v2 = (double4)(m2[m2Base + 4 * k], m2[m2Base + 1 + 4 * k], m2[m2Base + 2 + 4 * k], m2[m2Base + 3 + 4 * k]);
		sum4 += v1 * v2;
	}

	double sum = 0;
	m1Base += bound4 * 4;
	m2Base += bound4 * 4;
	int bound = (m - bound16 * 16 - bound4 * 4);

	for (int k = 0; k < bound; ++k)
	{
		sum += m1[m1Base] * m2[m2Base];
	}

	m3[i * l + j] = sum16.s0 + sum16.s1 + sum16.s2 + sum16.s3 + sum16.s4 + sum16.s5 + sum16.s6 + 
		sum16.s7 + sum16.s8 + sum16.s9 + sum16.sa + sum16.sb + sum16.sc + sum16.sd + sum16.se + 
		sum16.sf + sum4.s0 + sum4.s1 + sum4.s2 + sum4.s3 + sum;
}
