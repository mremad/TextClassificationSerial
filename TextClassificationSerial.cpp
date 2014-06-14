// TextClassificationSerial.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"
#include "LabelFactory.h"

#define DATA_PATH_EMAD "/Users/Mohamed/Desktop/TextClassificationSerial/DocumentClassifierData.txt"
#define DATA_PATH_MOUMEN
#define DATA_PATH_SHAABAN

int main(int argc, char* argv[])
{
    
	int document_size = 500;

	FileReader fr = FileReader(document_size,DATA_PATH_EMAD);
    
	FeatureConstructor fc = FeatureConstructor(fr.documents_size,document_size);
    fc.extract_vocab(fr.data_list, fr.documents_size, document_size);
    fc.construct_feature_vectors(fr.data_list, fr.documents_size, document_size);
    
	NaiveBayesClassifier nc = NaiveBayesClassifier(fc.NUM_OF_LABELS,fc.NUM_OF_UNIQUE_WORDS);
    nc.calculate_likelihood(fc.feature_vector,fc.NUM_OF_UNIQUE_WORDS,document_size,fc.NUM_OF_LABELS);
	nc.calculate_prior(fc.feature_vector,document_size,fc.NUM_OF_LABELS);
    
    
	int label = nc.classify_unlabeled_document(fc.feature_vector[9],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    label = nc.classify_unlabeled_document(fc.feature_vector[499],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    
    printf("Program terminated safely\n");
	getchar();
	return 0;
}

