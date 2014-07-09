#ifndef _EM
#define _EM
#include "NaiveBayesClassifier.h"
#include "ConsolePrint.h"

class EM
{
    void classify_all_unlabeled_documents(NaiveBayesClassifier* classifier, int* feature_vectors,int* docs_sizes, int* docs_ind, int number_unique_words, int number_documents, int number_labels, int* result);
    void copy_parameters(double** src_likelihood, double* src_prior, double** dest_likelihood, double* dest_prior, int number_labels, int number_unique_words);
	void copy_labels(int* new_labels, int* old_labels, int num_labels);
    bool check_if_converged(double** old_likelihood, double* old_prior, double** new_likelihood, double* new_prior, int number_labels, int number_unique_words);
	void build_seperate_fv(int* fv, int* docs_sizes, int* docs_ind, int* docs_labels, int* labeled_docs, int* unlabeled_docs, int num_labeled, int num_unlabeled);
	int	 calculate_size_feature_vector(int* docs_ind,int* docs_sizes, int num_docs);
	void initialize_weights(int num_labeled_docs, int num_unlabeled_docs, int num_labels);
	void estimate_weights(int* old_labels, int* new_labels, int num_docs);
	void update_total_weights(int num_unlabeled_docs, int num_labels);

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

	double* weights;
	double* weights_unlabeled;

	public:
		EM();
		void run_em(NaiveBayesClassifier* classifier, int* feature_vectors,int* docs_sizes, int* docs_ind,int* docs_labels, int* labeled_docs, int* unlabeled_docs, int number_unique_words,int number_unlabeled_documents, int number_labeled_documents, int number_labels);
    
};

#endif