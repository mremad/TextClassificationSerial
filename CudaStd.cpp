#include "CudaStd.h"
using namespace std;

int CudaStd::cuda_ver = CODE_CUDA;

double** CudaStd::convert_1d_to_2d_float(double* src, int rows, int cols)
{
	double** dest = (double**)malloc(rows*sizeof(double*));

	for(int i = 0;i < rows;i++)
	{
		dest[i] = (double*)malloc(cols*sizeof(double));

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

double* CudaStd::convert_2d_to_1d_float(double** src, int rows, int cols)
{
	double* dest = (double*)malloc(rows*cols*sizeof(double));

	for(int i = 0;i < rows;i++)
	{
		for(int j = 0;j < cols;j++)
		{
			dest[i*cols + j] = src[i][j];
		}

	}

	return dest;
}

string* CudaStd::convert_2d_to_1d_string(string** src, int rows, int* cols, int dst_size)
{
	string* dst = new string[dst_size];
	int index=0;
	//returns a 1D array of strings
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols[i];j++)
		{
			dst[index]=src[i][j];
			index++;
		}

	return dst;
}

char* CudaStd::convert_string_arr_to_char_arr(string* src, int num_strings, int dst_size,int* indexes_string_start)
{
	char* dst = (char*)malloc(sizeof(char)*dst_size);
	int index=0;
	//returns a 1D array of strings
	for(int i=0;i<num_strings;i++)
		{
			indexes_string_start[i]=index;
			
			for(int j=0;j<src[i].length();j++)
			{
				dst[index+j]= src[i].at(j);
			}
			index+=src[i].length();
		}

	return dst;
}