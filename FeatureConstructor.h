#ifndef _FEATURECONSTRUCTOR
#define _FEATURECONSTRUCTOR

#include <iostream>
#include <fstream>
#include <string>
#include "SuperHash.h"
#include "LinkedList.h"
using namespace std;



class FeatureConstructor
{
    
    
    LinkedList*		hash_list;
    int				max_List_Size;
    int				collissions;
    
    int		hash_str(string word, int tableSize);
    bool	check_if_feature(string word);
    int		get_index_for_label(string label);
    void	convert_labels_integers(string ** data_list, int number_documents);
	void	FeatureConstructor::extract_labels(string** data_list, int number_documents);
	int		FeatureConstructor::get_document_index(int* documents_size, int doc_num);
	
    public:
    FeatureConstructor();
    FeatureConstructor(int* document_size, int number_documents);
    

    //1D array of the labeled documents feature vectors. Size:DxN
    int *	feature_vector;
	int *	documents_labels;
    // 1D array of labels
    string*	label_list;
    //Number of all unique words
    int		num_unique_words;
    //Number of labels
    int		num_labels;
	//Number of words of each document
	int*	documents_size;
    
    void	extract_vocab(string** data_list, int* documents_size, int number_documents);
    //Should handle both cases of labeled/unlabeled
    void	construct_feature_vectors(string** data_list, int* documents_size, int number_documents);
};

#endif