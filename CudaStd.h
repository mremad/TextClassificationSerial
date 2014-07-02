#ifndef _CUDASTD
#define _CUDASTD
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;
class CudaStd
{
public:
	static long double**	convert_1d_to_2d_float(long double* src, int rows, int cols);
	static int**			convert_1d_to_2d_int(int* src, int rows, int cols);
	static long double*		convert_2d_to_1d_float(long double** src, int rows, int cols);
	static string*			convert_2d_to_1d_string(string** src, int rows, int cols);
};
#endif