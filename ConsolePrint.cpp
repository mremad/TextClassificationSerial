#include "ConsolePrint.h"

static void print_string_array(char** arr)
{
	
}

void ConsolePrint::print(char* arr)
{
	printf(arr);
}

void ConsolePrint::create_dummy_fv(int* vocab_size, int* document_size, int ** fv)
{


	for(int i = 0;i<*document_size;i++)
	{
		for(int j = 0;j<*vocab_size;j++)
		{
			fv[i][j] = i*(j+1) + j ;
		}
	}
}

void ConsolePrint::print_2d_int(int* vocab_size, int* document_size, int ** fv)
{
    printf("Printing Array: \n");
	for(int i = 0;i<*document_size;i++)
	{
		for(int j = 0;j<*vocab_size;j++)
		{
			printf("%i ",fv[i][j]);
		}
		printf("\n");
	}
    
    printf("\n");
}