// TextClassificationSerial.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"
#include "ConsolePrint.h"

int main(int argc, char* argv[])
{
	FileReader fr = FileReader();

	FeatureConstructor fc = FeatureConstructor();

	EM em = EM();

	fr.read_files();

	int doc_size;
	int vocab_size;
	int** fv;

	ConsolePrint::create_dummy_fv(&doc_size, &vocab_size, &fv);

	//printf("%i ",fv[1][1]);

	getchar();
	return 0;
}

