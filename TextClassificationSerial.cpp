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

	getchar();
	return 0;
}

