#include "NaiveBayesClassifier.h"

class EM
{
	public:
		EM();
		void run_em(NaiveBayesClassifier* classifier, int** labeled_feature_vectors, int** unlabeled_feature_vectors, int number_unique_words, int number_documents, int number_labels);
};