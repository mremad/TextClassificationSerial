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


	/*FeatureConstructor fc = FeatureConstructor();

	FileReader fr = FileReader(2,"/Users/moumenmohamed/Documents/Xcode/text2/text2/DocumentClassifierData.txt");

	FeatureConstructor fc = FeatureConstructor(fr.documents_size,2);
    fc.extract_vocab(fr.data_list, fr.documents_size, 2);
    fc.construct_feature_vectors(fr.data_list, fr.documents_size, 2);
    
    for(int i=0;i<fc.NUM_OF_UNIQUE_WORDS;i++)
    {
        //printf("%s",fc.vocab_list[i].c_str());
    }
    //printf("\n");
    //ConsolePrint::print_2d_int2(fc.NUM_OF_UNIQUE_WORDS, fc.NUM_OF_DOCUMENTS, fc.feature_vector);
    
    
>>>>>>> origin/master
    
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

	*///printf("Label: %d \n",label);
	//int tempFV[5][3]={{1,3,4},{2,7,3},{1,7,1},{2,8,2},{2,9,0}};
	/*int **FV;
	int desired_labeled=3;
	int total=5;
	//int labelList;
	FV = (int**)malloc(sizeof(int*)*5);
	for(int i=0;i<5;i++)
		FV[i] =(int*)malloc(sizeof(int)*3); 
	FV[0][0]=1;
	FV[0][1]=3;FV[3][1]=8;
	FV[0][2]=4;FV[3][2]=2;
	FV[1][0]=2;FV[4][0]=2;
	FV[1][1]=7;FV[4][1]=9;
	FV[1][2]=3;FV[4][2]=0;
	FV[2][0]=1;
	FV[2][1]=7;
	FV[2][2]=1;
	FV[3][0]=2;
	LabelFactory lf= LabelFactory(FV,total,desired_labeled,2);

    */
    printf("Program terminated safely\n");
	getchar();
	return 0;
}

