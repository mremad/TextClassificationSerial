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

void EM::copy_parameters(long double** src_likelihood, long double* src_prior, long double** dest_likelihood, long double* dest_prior, int number_labels, int number_unique_words)
{
    for(int i = 0;i<number_labels;i++)
    {
        for(int j = 0;j<number_unique_words;j++)
        {
            dest_likelihood[i][j] = src_likelihood[i][j];
        }
        
        dest_prior[i] = src_prior[i];
    }
}

bool EM::check_if_converged(long double** old_likelihood, long double* old_prior, long double** new_likelihood, long double* new_prior, int number_labels, int number_unique_words)
{
    bool result = false;
    
    long double total_diff = 0;
    
    long double threshold = 0.0001;
    
    for(int i = 0;i<number_labels;i++)
    {
        for(int j = 0;j<number_unique_words;j++)
        {
            total_diff += fabs(old_likelihood[i][j]-new_likelihood[i][j]);
        }
        
        total_diff += fabs(old_prior[i]-new_prior[i]);
    }
    
    printf("Total Difference: %Lf\n",total_diff);
    
    if(total_diff < threshold)
        result = true;
    
    return result;
}

void EM::run_em(NaiveBayesClassifier* classifier, int** feature_vectors, int** labeled_docs, int** unlabeled_docs, int number_unique_words,int number_unlabeled_documents, int number_labeled_documents, int number_labels)
{
    
    /*Initialize arrays to store old parameters*/
    
    long double** old_likelihood = (long double**)malloc(sizeof(long double*)*number_labels);
    long double* old_prior = (long double*)malloc(sizeof(long double)*number_labels);
    
    for(int i = 0;i<number_labels;i++)
        old_likelihood[i] = (long double*)malloc(sizeof(long double)*number_unique_words);
    
    
	/*Initial Step*/
	//Construct classifier with labeled feature_vectors
    
    classifier->calculate_likelihood(labeled_docs, number_unique_words, number_labeled_documents, number_labels);

    classifier->calculate_prior(labeled_docs, number_labeled_documents, number_labels);
    
	for(;;)
	{
        
		/*E Step*/
        
        printf("Performing E Step\n");
        classify_all_unlabeled_documents(classifier, unlabeled_docs, number_unique_words, number_unlabeled_documents, number_labels);
        
        //ConsolePrint::print_2d_int(number_unique_words, number_unlabeled_documents, unlabeled_docs);
        //ConsolePrint::print_2d_int(number_unique_words, number_labeled_documents, labeled_docs);
        //ConsolePrint::print_2d_int(number_unique_words, number_labeled_documents+number_unlabeled_documents, feature_vectors);
		
        /*M Step*/
        
        printf("Performing M Step\n");
        copy_parameters(classifier->get_likelihood(), classifier->get_prior(), old_likelihood, old_prior, number_labels, number_unique_words);
        
		classifier->calculate_likelihood(feature_vectors, number_unique_words, number_labeled_documents+number_unlabeled_documents, number_labels);

		classifier->calculate_prior(feature_vectors,number_labeled_documents+number_unlabeled_documents,number_labels);

		/*Check for convergence*/
        
        if(check_if_converged(old_likelihood,old_prior,classifier->get_likelihood(),classifier->get_prior(),number_labels,number_unique_words))
            break;
        
		
	}
    
    printf("EM Process Done\n");
}