#ifndef _EM
#define _EM
#include "NaiveBayesClassifier.h"
#include "ConsolePrint.h"

class EM
{
    void classify_all_unlabeled_documents(NaiveBayesClassifier* classifier, int* feature_vectors,int* docs_sizes, int* docs_ind, int number_unique_words, int number_documents, int number_labels, int* result);
    void copy_parameters(long double** src_likelihood, long double* src_prior, long double** dest_likelihood, long double* dest_prior, int number_labels, int number_unique_words);
    bool check_if_converged(long double** old_likelihood, long double* old_prior, long double** new_likelihood, long double* new_prior, int number_labels, int number_unique_words);
	void build_seperate_fv(int* fv, int* docs_sizes, int* docs_ind, int* docs_labels, int* labeled_docs, int* unlabeled_docs, int num_labeled, int num_unlabeled);
	int	 calculate_size_feature_vector(int* docs_ind,int* docs_sizes, int num_docs);

	int* labeled_fv;
	int* unlabeled_fv;

	int* labeled_docs_sizes;
	int* unlabeled_docs_sizes;

	int* labeled_docs_ind;
	int* unlabeled_docs_ind;

	int* labeled_doc_labels;
	int* unlabeled_doc_labels;
	int* all_doc_labels;

	int* labeled_docs;
	int* unlabeled_docs;

	public:
		EM();
		void run_em(NaiveBayesClassifier* classifier, int* feature_vectors,int* docs_sizes, int* docs_ind,int* docs_labels, int* labeled_docs, int* unlabeled_docs, int number_unique_words,int number_unlabeled_documents, int number_labeled_documents, int number_labels);
    
};

#endif