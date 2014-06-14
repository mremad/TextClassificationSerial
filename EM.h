#include "NaiveBayesClassifier.h"
#include "ConsolePrint.h"

class EM
{
    void classify_all_unlabeled_documents(NaiveBayesClassifier* classifier, int** feature_vectors, int number_unique_words, int number_documents, int number_labels);
    void copy_parameters(long double** src_likelihood, long double* src_prior, long double** dest_likelihood, long double* dest_prior, int number_labels, int number_unique_words);
    bool check_if_converged(long double** old_likelihood, long double* old_prior, long double** new_likelihood, long double* new_prior, int number_labels, int number_unique_words);
	public:
		EM();
		void run_em(NaiveBayesClassifier* classifier, int** feature_vectors, int** labeled_docs, int** unlabeled_docs, int number_unique_words,int number_unlabeled_documents, int number_labeled_documents, int number_labels);
    
};