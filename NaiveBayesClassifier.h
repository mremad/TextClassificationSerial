#ifndef _NAIVEBAYESCLASSIFIER
#define _NAIVEBAYESCLASSIFIER

#include <stdlib.h>
#include "stdio.h"
#include "math.h"
#include "Cfg.h"

class NaiveBayesClassifier
{
	

	#ifndef CUDA
	int calculate_single_word_label_occurence(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int word_index, int number_documents, int label);
	int calculate_all_words_label_occurence(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int number_unique_words, int number_documents, int label);
	
	#endif
	#ifndef CUDA_CLASSIFY
	long double prob_document_label(int * feature_vector, int fv_size, int number_unique_words, int label);
	#endif
	int calculate_label_occurance(int * documents_labels, int number_documents, int label);

    //First Bayes parameter, the prob. of each word per label. Size NxC
    long double ** first_parameter;
    //Second Bayes parameter, the prob. of each label. Size C
    long double * second_parameter;
    
	public:
		NaiveBayesClassifier(int number_labels,int number_unique_words);

        long double **	get_likelihood();
        long double *	get_prior();
		void			calculate_likelihood(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int number_unique_words, int number_documents, int number_labels);
		void			calculate_prior(int* documents_labels, int number_documents, int number_labels);
		#ifndef CUDA_CLASSIFY
		int				classify_unlabeled_document(int * unlabeled_feature_vector, int fv_size, int number_unique_words, int number_labels);
		#endif
		void			classify_unlabeled_documents(int* feature_vectors, int* docs_sizes, int* docs_indexes, int num_documents, int num_unique_words, int num_labels, int * labels);
};

#endif