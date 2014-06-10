#include "NaiveBayesClassifier.h"
#include "ConsolePrint.h"

class EM
{
    void classify_all_unlabeled_documents(NaiveBayesClassifier* classifier, int** feature_vectors, int number_unique_words, int number_documents, int number_labels);
	public:
		EM();
		void run_em(NaiveBayesClassifier* classifier, int** feature_vectors, int** labeled_docs, int** unlabeled_docs, int number_unique_words,int number_unlabeled_documents, int number_labeled_documents, int number_labels);
};