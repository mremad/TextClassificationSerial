#include "EM.h"

EM::EM()
{

}

void EM::classify_all_unlabeled_documents(NaiveBayesClassifier* classifier, int** feature_vectors, int number_unique_words, int number_documents, int number_labels)
{
    for(int i = 0;i < number_documents;i++)
    {
        int label = classifier->classify_unlabeled_document(feature_vectors[i], number_unique_words, number_labels);
        feature_vectors[i][0] = label;
    }
}

void EM::run_em(NaiveBayesClassifier* classifier, int** feature_vectors, int** labeled_docs, int** unlabeled_docs, int number_unique_words,int number_unlabeled_documents, int number_labeled_documents, int number_labels)
{
	/*Initial Step*/
	//Construct classifier with labeled feature_vectors
    
    classifier->calculate_first_parameter(labeled_docs, number_unique_words, number_labeled_documents, number_labels);

    classifier->calculate_second_parameter(labeled_docs, number_labeled_documents, number_labels);
    
	for(int i = 0; i < 10;i++)
	{
		/*E Step*/
        
        printf("Performing E Step\n");
        classify_all_unlabeled_documents(classifier, unlabeled_docs, number_unique_words, number_unlabeled_documents, number_labels);
        
        ConsolePrint::print_2d_int(number_unique_words, number_unlabeled_documents, unlabeled_docs);
        ConsolePrint::print_2d_int(number_unique_words, number_labeled_documents, labeled_docs);
        ConsolePrint::print_2d_int(number_unique_words, number_labeled_documents+number_unlabeled_documents, feature_vectors);
		/*M Step*/
        
        printf("Performing M Step\n");
		classifier->calculate_first_parameter(feature_vectors, number_unique_words, number_labeled_documents+number_unlabeled_documents, number_labels);

		classifier->calculate_second_parameter(feature_vectors,number_labeled_documents+number_unlabeled_documents,number_labels);

		/*Check for convergence*/
		//break;
		
	}
    
    printf("EM Process Done\n");
}