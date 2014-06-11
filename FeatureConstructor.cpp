 #include "FeatureConstructor.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

FeatureConstructor::FeatureConstructor()
{
    
}


FeatureConstructor::FeatureConstructor(int* document_size, int number_documents)
{
    NUM_OF_LABELS=0;
    NUM_OF_UNIQUE_WORDS=0;
    NUM_OF_DOCUMENTS= number_documents;
    int totalSize=0;
    for(int i=0;i<NUM_OF_DOCUMENTS;i++)
    {
        totalSize+= document_size[i];
    }
    
    vocab_list= new string[totalSize];
    label_list= new string[number_documents];
}

//Builds a list of all unique words in vocab_list from data_list
void FeatureConstructor::extract_vocab(string** data_list,int* documents_size, int number_documents)
{
  
    
    // loop over all documents and extract all labels
    
    for(int i=0;i<number_documents;i++)
    {
        
        bool found=false;
        // loop over all discovered labes see if it already added or not
        for(int l=0;l<NUM_OF_LABELS; l++)
        {
            if (data_list[i][0]== label_list[l])
            {
                // if found, set the flag to true and replace the lablel with its index in the labellist
                found= true;
                data_list[i][0]=l;
            }
        }
        
        // if the label isn't found in the label list
        if(!found)
        {
            // add it to the list
            label_list[NUM_OF_LABELS]= data_list[i][0];
            // replace the label with its index in
            data_list[i][0]= NUM_OF_LABELS;
            // increment the number of labels
            NUM_OF_LABELS++;
        }
    }
    
    
    // loop over documents
    for(int i=0;i<number_documents; i++)
    {
        // loop over all words in this document
        for(int j=1;j<documents_size[i];j++)
        {
            // search if the word is already in the vocab list
            bool found=false;
            for(int k=0;k<NUM_OF_UNIQUE_WORDS;k++)
            {
                // if word is found, set the flag to true
                if(data_list[i][j]== vocab_list[k])
                    found=true;
            }
            
            // if the word is new, add it to the vocab list
            if(!found)
            {
                // add new words to vocab and update number of unique words
                vocab_list[NUM_OF_UNIQUE_WORDS]= data_list[i][j];
                NUM_OF_UNIQUE_WORDS++;
            }
           
        }
    }
}



//Builds feature vectors for all documents in document_feature_vectors from data_list
void FeatureConstructor::construct_feature_vectors(string** data_list,int* documents_size, int number_documents)
{
    // set the number of rows to be equal number of documents
    feature_vector= new int* [NUM_OF_DOCUMENTS];
    
    // loop on every row and set number of columns to be equal of number of unique words
    for(int i=0;i<NUM_OF_DOCUMENTS;i++)
    {
        feature_vector[i]= new int [NUM_OF_UNIQUE_WORDS];
    }
    
    // loop over every document
    for(int i=0;i<NUM_OF_DOCUMENTS;i++)
    {
        // loop over every word in the vocab
        for(int j=0;j<NUM_OF_UNIQUE_WORDS;j++)
        {
            int count =0;
            //loop over all words in this document to see how many copy of this unique word exits
            for(int k=0;k<documents_size[i];k++)
            {
                if(vocab_list[j]== data_list[i][k])
                    count ++;
            }
            // update the value in the feature vector;
            feature_vector[i][j]=count;
        }
    }
    
    
    
}