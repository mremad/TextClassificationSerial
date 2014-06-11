#include <stdlib.h> 
#include <stdio.h>
#ifndef _CONSOLEPRINT
#define _CONSOLEPRINT
class ConsolePrint
{
	public:
		static void create_dummy_fv(int* vocab_size, int* document_size, int ** fv);
        static void print_2d_int(int* vocab_size, int* document_size, int ** fv);
     static void print_2d_int2(int vocab_size, int document_size, int ** fv);
		static void print(char* arr);
};
#endif