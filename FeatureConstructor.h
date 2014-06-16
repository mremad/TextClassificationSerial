#include <iostream>
#include <fstream>
#include <string>
#include "superHash.h"
#include "LinkedList.h"
using namespace std;



class FeatureConstructor
{
    bool check_if_feature(string word);
	public:
    FeatureConstructor();
    FeatureConstructor(int* document_size, int number_documents);
		//1D array of the unique vocabulary. Size: N
		string* vocab_list;
    
        LinkedList* hash_list;
        int max_List_Size;
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
    
    int collissions;
    
        //uint32_t create_hash( std::string data, int len);
        //uint32_t hash_inc(const char * data, int len, uint32_t hash);
    
        int hash_str(string word, int tableSize);
    
		void extract_vocab(string** data_list, int* documents_size, int number_documents);

		//Should handle both cases of labeled/unlabeled
		void construct_feature_vectors(string** data_list, int* documents_size, int number_documents);
};