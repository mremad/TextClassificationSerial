// TextClassificationSerial.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"
#include "LabelFactory.h"

int main(int argc, char* argv[])
{

	int document_size = 20;
	int vocab_size = 5;
	int number_classes = 20;
	int** fv;
    int** l_fv;
    int** u_fv;
    int l_docs = 10;
    int u_docs = 10;

	fv = (int**) malloc(sizeof(int*)*(document_size));
    l_fv = (int**) malloc(sizeof(int*)*(l_docs));
    u_fv = (int**) malloc(sizeof(int*)*(u_docs));
    
	for(int i = 0;i<document_size;i++)
	{
		fv[i] = (int*) malloc(sizeof(int)*(vocab_size));
	}

	FileReader fr = FileReader(10,"DocumentClassifierData.txt");

	FeatureConstructor fc = FeatureConstructor();
    
    LabelFactory lf = LabelFactory();

	EM em = EM();

	NaiveBayesClassifier nc = NaiveBayesClassifier(number_classes,vocab_size);

	//fr.read_files();



	ConsolePrint::create_dummy_fv(&vocab_size, &document_size, fv);
    
    for(int i = 0;i<l_docs;i++)
    {
        l_fv[i] = fv[i*2];
        u_fv[i] = fv[(i*2)+1];
    }
    
    
    ConsolePrint::print_2d_int(&vocab_size, &document_size, fv);
    ConsolePrint::print_2d_int(&vocab_size, &l_docs, l_fv);
    ConsolePrint::print_2d_int(&vocab_size, &u_docs, u_fv);
    

    em.run_em(&nc, fv, l_fv, u_fv, vocab_size, u_docs, l_docs, number_classes);
	//nc.calculate_first_parameter(fv,vocab_size,document_size,number_classes);
	//nc.calculate_second_parameter(fv,document_size,number_classes);

	//int label = nc.classify_unlabeled_document(fv[9],vocab_size,number_classes);

	//printf("Label: %d \n",label);
    
    printf("Program terminated safely\n");
	getchar();
	return 0;
}

