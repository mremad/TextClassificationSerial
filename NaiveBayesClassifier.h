#ifndef _NAIVEBAYESCLASSIFIER
#define _NAIVEBAYESCLASSIFIER

#include <stdlib.h>
#include "stdio.h"
#include "math.h"
class NaiveBayesClassifier
{
	long double prob_document_label(int * feature_vector, int number_unique_words, int label);
	long double prob_document_all_labels(int * feature_vector, int number_unique_words, int number_labels);

	int calculate_single_word_label_occurence(int ** feature_vectors, int word_index, int number_documents, int label);
	int calculate_all_words_label_occurence(int ** feature_vectors, int number_unique_words, int number_documents, int label);

	int calculate_label_occurance(int ** feature_vectors, int number_documents, int label);

    //First Bayes parameter, the prob. of each word per label. Size NxC
    long double ** first_parameter;
    //Second Bayes parameter, the prob. of each label. Size C
    long double * second_parameter;
    
	public:
		NaiveBayesClassifier(int number_labels,int number_unique_words);

        long double ** get_likelihood();
        long double *  get_prior();
		void calculate_likelihood(int** feature_vectors,int number_unique_words, int number_documents, int number_labels);
		void calculate_prior(int** feature_vectors, int number_documents, int number_labels);
		int classify_unlabeled_document(int * unlabeled_feature_vector, int number_unique_words, int number_labels);
};

#endif