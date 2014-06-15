#ifndef _FEATURECONSTRUCTOR
#define _FEATURECONSTRUCTOR

#include <iostream>
#include <fstream>
#include <string>

using namespace std;



class FeatureConstructor
{
    bool check_if_feature(string word);
    
    int * documents_labels;
    
    int get_index_for_label(string label);
    void convert_labels_integers(string ** data_list, int number_documents);
    
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

#endif