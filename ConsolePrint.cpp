#include "ConsolePrint.h"


void ConsolePrint::print_string(std::string arr)
{
	printf("%s\n",arr.c_str());
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

void ConsolePrint::print_1d_float(int num_columns, double * fv)
{
    printf("Printing double Array: \n");
    for(int j = 0;j<num_columns;j++)
    {
        printf("%Lf\t",fv[j]);
    }
    printf("\n");
    
}

void ConsolePrint::print_1d_int(int vocab_size, int * fv)
{
    printf("Printing Array: \n");
    for(int j = 0;j<vocab_size;j++)
    {
        printf("%i ",fv[j]);
    }
    printf("\n");
}

void ConsolePrint::print_2d_float(int num_columns, int num_rows, double ** fv)
{
    printf("Printing double Array: \n");
	for(int i = 0;i<num_rows;i++)
	{
		for(int j = 0;j<num_columns;j++)
		{
			printf("%Lf\t",fv[i][j]);
		}
		printf("\n");
	}
    
    printf("\n");
}


void ConsolePrint::print_2d_int(int vocab_size, int document_size, int ** fv)
{
    printf("Printing Array: \n");
	for(int i = 0;i<document_size;i++)
	{
		for(int j = 0;j<vocab_size;j++)
		{
			printf("%i ",fv[i][j]);
		}
		printf("\n");
	}
    
    printf("\n");
}