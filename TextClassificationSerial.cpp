// TextClassificationSerial.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"
#include "LabelFactory.h"

int main(int argc, char* argv[])
{

	int document_size = 500;
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


	

	FileReader fr = FileReader(document_size,"/Users/Mohamed/Desktop/TextClassificationSerial/DocumentClassifierData.txt");

    
	FeatureConstructor fc = FeatureConstructor(fr.documents_size,document_size);
    fc.extract_vocab(fr.data_list, fr.documents_size, document_size);
    fc.construct_feature_vectors(fr.data_list, fr.documents_size, document_size);
    
    for(int i=0;i<fc.NUM_OF_UNIQUE_WORDS;i++)
    {
        printf("%s ",fc.vocab_list[i].c_str());
    }
    printf("\n");
    //ConsolePrint::print_2d_int(fc.NUM_OF_UNIQUE_WORDS, fc.NUM_OF_DOCUMENTS, fc.feature_vector);
    
    NaiveBayesClassifier nc = NaiveBayesClassifier(fc.NUM_OF_LABELS,fc.NUM_OF_UNIQUE_WORDS);
    nc.calculate_first_parameter(fc.feature_vector, fc.NUM_OF_UNIQUE_WORDS, fc.NUM_OF_DOCUMENTS, fc.NUM_OF_LABELS);
    nc.calculate_second_parameter(fc.feature_vector, fc.NUM_OF_DOCUMENTS, fc.NUM_OF_LABELS);
    
    ConsolePrint::print_2d_float(10, fc.NUM_OF_LABELS, nc.first_parameter);
    
 /*
    LabelFactory lf = LabelFactory();

	EM em = EM();

	

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

	*///printf("Label: %d \n",label);
	//int tempFV[5][3]={{1,3,4},{2,7,3},{1,7,1},{2,8,2},{2,9,0}};
    
    printf("Program terminated safely\n");
	getchar();
	return 0;
}

