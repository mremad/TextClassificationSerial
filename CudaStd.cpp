#include "CudaStd.h"
using namespace std;
long double** CudaStd::convert_1d_to_2d_float(long double* src, int rows, int cols)
{
	long double** dest = (long double**)malloc(rows*sizeof(long double*));

	for(int i = 0;i < rows;i++)
	{
		dest[i] = (long double*)malloc(cols*sizeof(long double));

		for(int j = 0;j < cols;j++)
		{
			dest[i][j] = src[i*cols + j];
		}

	}

	return dest;
}

int** CudaStd::convert_1d_to_2d_int(int* src, int rows, int cols)
{
	int** dest = (int**)malloc(rows*sizeof(int*));

	for(int i = 0;i < rows;i++)
	{
		dest[i] = (int*)malloc(cols*sizeof(int));

		for(int j = 0;j < cols;j++)
		{
			dest[i][j] = src[i*cols + j];
		}

	}

	return dest;
}

long double* CudaStd::convert_2d_to_1d_float(long double** src, int rows, int cols)
{
	long double* dest = (long double*)malloc(rows*cols*sizeof(long double));

	for(int i = 0;i < rows;i++)
	{
		for(int j = 0;j < cols;j++)
		{
			dest[i*cols + j] = src[i][j];
		}

	}

	return dest;
}

string* CudaStd::convert_2d_to_1d_string(string** src, int rows, int cols)
{
	string* dst = new string[rows*cols];

	//TODO: returns a 1D array of strings

	return dst;
}