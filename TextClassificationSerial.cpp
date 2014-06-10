// TextClassificationSerial.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"

int main(int argc, char* argv[])
{

	int document_size = 10;
	int vocab_size = 5;
	int number_classes = 10;
	int** fv;

	fv = (int**) malloc(sizeof(int*)*(document_size));
	for(int i = 0;i<document_size;i++)
	{
		fv[i] = (int*) malloc(sizeof(int)*(vocab_size));
	}

	//FileReader fr = FileReader(10,0);

	FeatureConstructor fc = FeatureConstructor();

	EM em = EM();

	NaiveBayesClassifier nc = NaiveBayesClassifier(number_classes,vocab_size);

	//fr.read_files();



	ConsolePrint::create_dummy_fv(&vocab_size, &document_size, fv);

	nc.calculate_first_parameter(fv,vocab_size,document_size,number_classes);
	nc.calculate_second_parameter(fv,document_size,number_classes);

	int label = nc.classify_unlabeled_document(fv[9],vocab_size,number_classes);

	printf("Label: %d \n",label);

	getchar();
	return 0;
}

