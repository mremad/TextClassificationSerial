class NaiveBayesClassifier
{
	public:
		//First Bayes parameter, the prob. of each word per label. Size NxC
		float ** first_parameter;
		//Second Bayes parameter, the prob. of each label. Size C
		float * second_parameter;

		void calculate_first_parameter(int** feature_vectors,int number_unique_words, int number_documents, int number_labels);
		void calculate_second_parameter(int** feature_vectors, int number_documents, int number_labels);
		int classify_unlabeled_document(int * unlabeled_feature_vector);
}