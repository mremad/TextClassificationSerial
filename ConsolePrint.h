#include <stdlib.h> 
#include <stdio.h>

class ConsolePrint
{
	public:
		static void create_dummy_fv(int* vocab_size, int* document_size, int ** fv);
		static void print(char* arr); 
};