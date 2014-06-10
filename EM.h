#include "NaiveBayesClassifier.h"

class EM
{
	public:
		EM();
		void run_em(NaiveBayesClassifier* classifier, int** feature_vectors, int number_unique_words,int* index_labeled_docs, int number_labeled_documents, int number_labels);
};