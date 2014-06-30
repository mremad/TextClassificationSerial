// TextClassificationSerial.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"
#include "LabelFactory.h"
#include "TestResults.h"
#include "Cfg.h"


int main(int argc, char* argv[])
{
    int start_time;
	int end_time;
	int document_size = 11290; // max 11290
    int desired_labeled = 40; // max 5500, must be a multiple of num_labels
    int num_labels = 20;
	int desired_unlabeled=40;//must be a multiple of num_labels
    
    int test_documents = 7500;

	FileReader fr = FileReader(document_size,DATA_PATH_SHAABAN);
    fr.read_files();
	string** dataU;
	string** dataL;
	dataU = (string**) malloc(sizeof(string*)*desired_labeled);
	dataL = (string**) malloc(sizeof(string*)*desired_unlabeled);
	
    fr.label_factory(desired_labeled, desired_unlabeled, num_labels,dataU,dataL);
	
	FeatureConstructor fc = FeatureConstructor(fr.documents_size_unlabeled,desired_unlabeled);
    fc.extract_vocab(dataU, fr.documents_size_unlabeled, desired_unlabeled);
    fc.construct_feature_vectors(dataU, fr.documents_size_unlabeled, desired_unlabeled);
    
	fr.deallocate();
    //LabelFactory lf = LabelFactory();
    //lf.select_labeled_docs(fc.feature_vector, fc.NUM_OF_DOCUMENTS, desired_labeled, fc.NUM_OF_LABELS);
    
	//NaiveBayesClassifier nc = NaiveBayesClassifier(fc.num_labels,fc.num_unique_words);
    
	//start_time = clock();
	//nc.calculate_likelihood(fc.feature_vector, fc.num_unique_words, document_size, fc.num_labels);
	//end_time = clock();
	//printf("Time to calculate: %f\n",((end_time - start_time)/(float)CLOCKS_PER_SEC));
	//nc.calculate_prior(fc.feature_vector, desired_labeled, fc.num_labels);
    
	//ConsolePrint::print_2d_float(fc.NUM_OF_UNIQUE_WORDS, 1, nc.get_likelihood());
    
    /*EM em = EM();
    em.run_em(&nc, fc.feature_vector , lf.labeled_fv, lf.unlabeled_fv, fc.NUM_OF_UNIQUE_WORDS, fc.NUM_OF_DOCUMENTS-desired_labeled, desired_labeled, fc.NUM_OF_LABELS);*/
    
   // TestResults tr = TestResults(TEST_PATH_SHAABAN, test_documents, &fc, &nc);
   // tr.start_test();
    
	//int label = nc.classify_unlabeled_document(fc.feature_vector[0],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    //printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    //label = nc.classify_unlabeled_document(fc.feature_vector[499],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    //printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    /*label = nc.classify_unlabeled_document(fc.feature_vector[1000],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    label = nc.classify_unlabeled_document(fc.feature_vector[1499],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    label = nc.classify_unlabeled_document(fc.feature_vector[2000],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    label = nc.classify_unlabeled_document(fc.feature_vector[2500],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    label = nc.classify_unlabeled_document(fc.feature_vector[2999],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);*/
    
    
    printf("Program terminated safely\n");
	getchar();
	return 0;
}

