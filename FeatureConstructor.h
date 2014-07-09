#ifndef _FEATURECONSTRUCTOR
#define _FEATURECONSTRUCTOR

#include <iostream>
#include <fstream>
#include <string>
#include "SuperHash.h"
#include "LinkedList.h"
#include "CudaStd.h"
using namespace std;


struct dataCollection {
  char* d_charDataList;
  int* d_indexes_string_start;
  char* d_HashArray; 
  int* d_words_per_hash_row;
  int* d_word_index_in_vocab;
  int total_word_count;
  int total_char_count;
  int hash_table_size;
  int hash_row_size;
  int hash_word_size;

} ;

class FeatureConstructor
{
    
    
    LinkedList*		hash_list;
    char *          h_hash_array;
    int *           h_words_per_hash_row;
    int *           startPos_in_hash_row;
	int*			h_word_index_in_vocab;
	
    int				max_List_Size;
    int				collissions;

    
    int		hash_str(string word, int tableSize);
    bool	check_if_feature(string word);
    int		get_index_for_label(string label);
    void	convert_labels_integers(string ** data_list, int number_documents);
	
	int		get_document_index(int* documents_size, int doc_num);
	void	extract_documents_indexes(int* documents_size,int number_documents);

public:
    FeatureConstructor();
	FeatureConstructor(int* document_size, int number_documents); 

    //1D array of the labeled documents feature vectors. Size:DxN
    int *	feature_vector;
	int	*	documents_indexes;
	int *	documents_labels;
    // 1D array of labels
    string*	label_list;
    //Number of all unique words
    int		num_unique_words;
    //Number of labels
    int		num_labels;
	//Number of words of each document
	int*	documents_size;
	// Number of characters in all documents read
	int total_char_count;
	// Number of words in all documents read
	int total_word_count;
    
    void	extract_vocab(string** data_list, int* documents_size, int number_documents);
	void	extract_vocab(string** data_list,int* documents_size, int number_documents, int total_word_count, int total_char_count);

	void	extract_labels(string** data_list, int number_documents);
	void	process_data_list(string** data_list, int* documents_size, int number_documents);
    
	//Should handle both cases of labeled/unlabeled
    void	construct_feature_vectors(string** data_list, int* documents_size, int number_documents);
	
};

#endif