class FeatureConstructor
{
	public:
		FeatureConstructor()
		{

		}
		//1D array of the unique vocabulary. Size: N
		char** vocab_list;
		//2D array of the labeled documents feature vectors. Size:NxD
		int ** labeled_feature_vectors;
		//2D array of the unlabeled documents feature vectors. Size:NxD
		int ** unlabeled_feature_vectors;
		//Number of all unique words
		int N;
		//Number of all documents
		int D;
		//Number of labels
		int C;

		void extract_vocab(char*** data_list, int* documents_size, int number_documents);

		//Should handle both cases of labeled/unlabeled
		void construct_feature_vectors(char*** data_list, int* documents_size, int number_documents);
}