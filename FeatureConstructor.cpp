#include "FeatureConstructor.h"

#define HASH_TABLE_SIZE 2039

using namespace std;

FeatureConstructor::FeatureConstructor()
{
    
}


int calculate_table_size(int* documents_size, int number_documents)
{
    int result=0;
    for(int i=0;i<number_documents;i++)
        result+=documents_size[i];
    return  result;
}

FeatureConstructor::FeatureConstructor(int* document_size, int number_documents)
{
    NUM_OF_LABELS=0;
    NUM_OF_UNIQUE_WORDS=0;
    NUM_OF_DOCUMENTS= number_documents;
    int totalSize=calculate_table_size(document_size, number_documents);
    
    vocab_list= new string[totalSize];
    label_list= new string[number_documents];
    hash_list= new LinkedList[HASH_TABLE_SIZE];
    max_List_Size=0;
    for(int i=0;i<HASH_TABLE_SIZE;i++)
    {
        hash_list[i]= *new LinkedList();
    }
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

int FeatureConstructor::hash_str(string word, int tableSize)
{
    int h = 31 /* also prime */;
    
    char * charList= (char *)malloc(word.length()*sizeof(char));
    for(int i=0;i<word.length();i++)
        charList[i]= word[i];
    
    
    while (*charList) {
       // h = (h * A) ^ (charList[0] * B);
        charList++;
    }
    return h%tableSize; // or return h % C;
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
        if(!found && data_list[i][0] != "")
        {
            string s = data_list[i][0];
            printf("Index: %d\tLabel: %s\tDoc: %d\n",NUM_OF_LABELS,data_list[i][0].c_str(),i);
            // add it to the list
            label_list[NUM_OF_LABELS]= data_list[i][0];
            
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
            
            // calculate the hash index
            int hashIndex= SuperHash::create_hash(data_list[i][j], (int)data_list[i][j].length(),HASH_TABLE_SIZE);
            // if the word wasn't already added to the list in the specified index
            if(!hash_list[hashIndex].Exists(data_list[i][j]))
            {
                // append word to the list
                hash_list[hashIndex].Append((data_list[i][j]),NUM_OF_UNIQUE_WORDS);
                // update the vocablist
                vocab_list[NUM_OF_UNIQUE_WORDS]=data_list[i][j];
                NUM_OF_UNIQUE_WORDS++;
            }
           
        }
    }
    
    /*
     // to print the array of lists
    for(int i=0;i<HASH_TABLE_SIZE;i++)
    {
        hash_list[i].Print();
        printf("\n");
        
    }*/
    
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
    int hashIndex, position;
    // set the number of rows to be equal number of documents
    feature_vector=  (int**)malloc(sizeof(int*)*number_documents);
    documents_labels = (int*)malloc(sizeof(int)*number_documents);
    convert_labels_integers(data_list, number_documents);
    

    
    // loop on every row and set number of columns to be equal of number of unique words
    for(int i=0;i<number_documents;i++)
    {
        
		feature_vector[i]=(int*)malloc(sizeof(int)*(NUM_OF_UNIQUE_WORDS+1));
		for(int j=0;j<NUM_OF_UNIQUE_WORDS+1;j++)
		{
			feature_vector[i][j] = 0;
		}
    }
    
    // loop over every document
    for(int i=0;i<number_documents;i++)
    {
        for(int j=0;j<documents_size[i];j++)
        {
            // check if word can be used as feature word
            if(!check_if_feature(data_list[i][j]))
                continue;
            // calculate index
            hashIndex= SuperHash::create_hash(data_list[i][j], (int)data_list[i][j].length(),HASH_TABLE_SIZE);
            // get the position of the word int he vocablist
            position= hash_list[hashIndex].getPositionValue(data_list[i][j]);
            if(position==-1)
                continue;
            else
            {
                // increment value in feature vector
                feature_vector[i][position+1]++;
            }
            
        }
        
        feature_vector[i][0] = documents_labels[i];
        
    }
    
    printf("Ended Feature Construction\n");
    
}