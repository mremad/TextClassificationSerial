// TextClassificationSerial.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"
#include "LabelFactory.h"

#define DATA_PATH_EMAD "/Users/Mohamed/Desktop/TextClassificationSerial/DocumentClassifierData.txt"
#define DATA_PATH_MOUMEN
#define DATA_PATH_SHAABAN "DocumentClassifierData.txt"

int main(int argc, char* argv[])
{
    
	int document_size = 750;
    int desired_labeled = 40;

	FileReader fr = FileReader(document_size,DATA_PATH_SHAABAN);
    
	FeatureConstructor fc = FeatureConstructor(fr.documents_size,document_size);
    fc.extract_vocab(fr.data_list, fr.documents_size, document_size);
    fc.construct_feature_vectors(fr.data_list, fr.documents_size, document_size);
    
    LabelFactory lf = LabelFactory();
    lf.select_labeled_docs(fc.feature_vector, fc.NUM_OF_DOCUMENTS, desired_labeled, fc.NUM_OF_LABELS);
    
	NaiveBayesClassifier nc = NaiveBayesClassifier(fc.NUM_OF_LABELS,fc.NUM_OF_UNIQUE_WORDS);
    
    EM em = EM();
    em.run_em(&nc, fc.feature_vector , lf.labeled_fv, lf.unlabeled_fv, fc.NUM_OF_UNIQUE_WORDS, fc.NUM_OF_DOCUMENTS-desired_labeled, desired_labeled, fc.NUM_OF_LABELS);
    
	int label = nc.classify_unlabeled_document(fc.feature_vector[0],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
    label = nc.classify_unlabeled_document(fc.feature_vector[499],fc.NUM_OF_UNIQUE_WORDS,fc.NUM_OF_LABELS);
    printf("Number of labels: %d Chosen Label: %d\n",fc.NUM_OF_LABELS,label);
    
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

