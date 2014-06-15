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

bool FeatureConstructor::check_if_feature(string word)
{
    bool check = true;
    
    if(word.length() <= 3)
    {
        check = false;
    }
    else if(word == "though" || word == "they" || word == "that" || word == "this" || word == "there" || word == "were"
            || word == "than" || word == "rather" || word == "from" || word == "most")
        check = false;
    
    return check;
}

//Builds a list of all unique words in vocab_list from data_list
void FeatureConstructor::extract_vocab(string** data_list,int* documents_size, int number_documents)
{
  
    
    // loop over all documents and extract all labels
    printf("Began Vocab Extraction\n");
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
            }
        }
        
        // if the label isn't found in the label list
        if(!found)
        {
            // add it to the list
            label_list[NUM_OF_LABELS]= data_list[i][0];
            // replace the label with its index in
            // increment the number of labels
            NUM_OF_LABELS++;
        }
    }
    printf("Labels found: %d\n",NUM_OF_LABELS);
    
    // loop over documents
    for(int i=0;i<number_documents; i++)
    {
        if( ((float)i/number_documents) == ((float)80/100) )
            printf("80%% Completed: %d Unique words found: %d\n",i,NUM_OF_UNIQUE_WORDS);
        else if(((float)i/number_documents) == ((float)60/100))
            printf("60%% Completed: %d Unique words found: %d\n",i,NUM_OF_UNIQUE_WORDS);
        else if(((float)i/number_documents) == ((float)40/100))
            printf("40%% Completed: %d Unique words found: %d\n",i,NUM_OF_UNIQUE_WORDS);
        else if(((float)i/number_documents) == ((float)20/100))
            printf("20%% Completed: %d Unique words found: %d\n",i,NUM_OF_UNIQUE_WORDS);
        else if(((float)i/number_documents) == ((float)10/100))
            printf("10%% Completed: %d Unique words found: %d\n",i,NUM_OF_UNIQUE_WORDS);
        else if(i == number_documents - 1)
            printf("100%% Completed: %d Unique words found: %d\n",i,NUM_OF_UNIQUE_WORDS);
            
        // loop over all words in this document
        for(int j=1;j<documents_size[i];j++)
        {
            if(!check_if_feature(data_list[i][j]))
                continue;
            
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
    
    printf("Ended Vocab Extraction\n");
}

int FeatureConstructor::get_index_for_label(string label)
{
    int index = -1;
    
    for(int i = 0;i < NUM_OF_LABELS;i++)
    {
        if(label == label_list[i])
        {
            index = i;
            break;
        }
    }
    
    return index;
}

void FeatureConstructor::convert_labels_integers(string ** data_list, int number_documents)
{
    for(int i = 0;i<number_documents;i++)
    {
        documents_labels[i] = get_index_for_label(data_list[i][0]);
    }
}



//Builds feature vectors for all documents in document_feature_vectors from data_list
void FeatureConstructor::construct_feature_vectors(string** data_list,int* documents_size, int number_documents)
{
    printf("Began Feature Construction\n");
    // set the number of rows to be equal number of documents
    feature_vector= new int* [number_documents];
    documents_labels = (int*)malloc(sizeof(int)*number_documents);
    convert_labels_integers(data_list, number_documents);
    
    // loop on every row and set number of columns to be equal of number of unique words
    for(int i=0;i<number_documents;i++)
    {
        feature_vector[i]= new int [NUM_OF_UNIQUE_WORDS+1];
    }
    
    // loop over every document
    for(int i=0;i<number_documents;i++)
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
            feature_vector[i][j+1]=count;
        }
        
        feature_vector[i][0] = documents_labels[i];
    }
    
    printf("Ended Feature Construction\n");
    
}