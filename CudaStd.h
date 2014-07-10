#ifndef _CUDASTD
#define _CUDASTD
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Cfg.h"

using namespace std;
class CudaStd
{
public:
	static int cuda_ver;

	static double**	convert_1d_to_2d_float(double* src, int rows, int cols);
	static int**			convert_1d_to_2d_int(int* src, int rows, int cols);
	static double*		convert_2d_to_1d_float(double** src, int rows, int cols);
	static string*			convert_2d_to_1d_string(string** src, int rows, int* cols, int dst_size);
	static char*			convert_string_arr_to_char_arr(string* src, int num_strings, int dst_size,int* indexes_string_start);
};
#endif