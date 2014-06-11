#include <iostream>
#include <fstream>
#include <string>

using namespace std;



class FeatureConstructor
{
	public:
    FeatureConstructor();
    FeatureConstructor(int* document_size, int number_documents);
		//1D array of the unique vocabulary. Size: N
		string* vocab_list;
		//2D array of the labeled documents feature vectors. Size:DxN
		int ** feature_vector;
    
        // 1D array of labels
        string* label_list;
		//Number of all unique words
		int NUM_OF_UNIQUE_WORDS;
		//Number of all documents
		int NUM_OF_DOCUMENTS;
		//Number of labels
		int NUM_OF_LABELS;
    
		void extract_vocab(string** data_list, int* documents_size, int number_documents);

		//Should handle both cases of labeled/unlabeled
		void construct_feature_vectors(string** data_list, int* documents_size, int number_documents);
};