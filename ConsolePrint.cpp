#include "ConsolePrint.h"

static void print_string_array(char** arr)
{
	
}

void ConsolePrint::create_dummy_fv(int* vocab_size, int* document_size, int* ** p_fv)
{
	*vocab_size = 10;
	*document_size = 5;
	int ** fv = *p_fv;
	fv = (int**) malloc(sizeof(int*)*(*document_size));
	for(int i = 0;i<*document_size;i++)
	{
		fv[i] = (int*) malloc(sizeof(int)*(*vocab_size));
	}

	for(int i = 0;i<*document_size;i++)
	{
		for(int j = 0;j<*vocab_size;j++)
		{
			fv[i][j] = i*j+j;
			printf("%i ",fv[i][j]);
		}
		printf("\n");
	}
}