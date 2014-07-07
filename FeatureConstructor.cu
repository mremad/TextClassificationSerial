#include "FeatureConstructor.h"
#include "Cfg.h"

using namespace std;

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
|| defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
+(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

 

dataCollection dataCollection_new()
{
	 dataCollection a;

  return a;
}

FeatureConstructor::FeatureConstructor()
{
    
}

FeatureConstructor::FeatureConstructor(int* document_size, int number_documents)
{
	
    label_list= new string[MAX_NUM_LABELS];
    num_labels=0;
	num_unique_words=0;
}


int calculate_table_size(int* documents_size, int number_documents)
{
    int result=0;
    for(int i=0;i<number_documents;i++)
        result+=documents_size[i];
    return  result;
}

FeatureConstructor::FeatureConstructor(int* document_size, int number_documents,int total_characters_count,int total_words_count)
{
    num_labels=0;
    num_unique_words=0;
	documents_size = document_size;

    total_char_count=total_characters_count;
	total_words_count= total_words_count;
	vocab_list= new string[total_words_count];
    label_list= new string[MAX_NUM_LABELS];
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

void FeatureConstructor::extract_labels(string** data_list, int number_documents)
{
	for(int i=0;i<number_documents;i++)
    {
		bool found=false;
        // loop over all discovered labes see if it already added or not
        for(int l=0;l<num_labels; l++)
        {
            if (data_list[i][0] == label_list[l])
            {
                // if found, set the flag to true and replace the lablel with its index in the labellist
                found= true;
            }
        }
        
        // if the label isn't found in the label list
        if(!found && data_list[i][0] != "")
        {
            string s = data_list[i][0];
            printf("Index: %d\tLabel: %s\tDoc: %d\n",num_labels,data_list[i][0].c_str(),i);
            // add it to the list
            label_list[num_labels]= data_list[i][0];
            
            // increment the number of labels
            num_labels++;
            
        }
    }
    printf("Labels found: %d\n",num_labels);
}

void FeatureConstructor::extract_documents_indexes(int* documents_size, int number_documents)
{
	documents_indexes = (int*)malloc(sizeof(int)*number_documents);

	for(int i = 0;i < number_documents;i++)
	{
		documents_indexes[i] = get_document_index(documents_size,i);
	}
}

//Builds a list of all unique words in vocab_list from data_list
void FeatureConstructor::extract_vocab(string** data_list,int* documents_size, int number_documents)
{
    
    // loop over all documents and extract all labels
    printf("Began Vocab Extraction\n");
    
	extract_labels(data_list,number_documents);

    // loop over documents
    for(int i=0;i<number_documents; i++)
    {
        if( ((float)i/number_documents) == ((float)80/100) )
            printf("80%% Completed: %d Unique words found: %d\n",i,num_unique_words);
        else if(((float)i/number_documents) == ((float)60/100))
            printf("60%% Completed: %d Unique words found: %d\n",i,num_unique_words);
        else if(((float)i/number_documents) == ((float)40/100))
            printf("40%% Completed: %d Unique words found: %d\n",i,num_unique_words);
        else if(((float)i/number_documents) == ((float)20/100))
            printf("20%% Completed: %d Unique words found: %d\n",i,num_unique_words);
        else if(((float)i/number_documents) == ((float)10/100))
            printf("10%% Completed: %d Unique words found: %d\n",i,num_unique_words);
        else if(i == number_documents - 1)
            printf("100%% Completed: %d Unique words found: %d\n",i,num_unique_words);
            
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
                hash_list[hashIndex].Append((data_list[i][j]),num_unique_words);
				// update the vocablist
				vocab_list[num_unique_words]=data_list[i][j];
				//printf("%s ",data_list[i][j]);
                num_unique_words++;
            }
           
        }
    }

	//printf("\n");
    
    printf("Ended Vocab Extraction\n");
}

int FeatureConstructor::get_index_for_label(string label)
{
    int index = -1;
    
    for(int i = 0;i < num_labels;i++)
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

int FeatureConstructor::get_document_index(int* documents_size, int doc_num)
{
	int index = 0;

	for(int i = 0;i<doc_num;i++)
	{
		index += documents_size[i];
	}

	return index;
}

//Builds feature vectors for all documents in document_feature_vectors from data_list
void FeatureConstructor::construct_feature_vectors(string** data_list,int* documents_size, int number_documents)
{
    printf("Began Feature Construction\n");
    int hashIndex, position;
	int totalSize = calculate_table_size(documents_size,number_documents);

    // set the number of rows to be equal number of documents
    feature_vector =  (int*)malloc(sizeof(int)*totalSize);
    documents_labels = (int*)malloc(sizeof(int)*number_documents);

    convert_labels_integers(data_list, number_documents);
	extract_documents_indexes(documents_size, number_documents);
    
    // loop on every row and set number of columns to be equal of number of unique words
    for(int i=0;i<totalSize;i++)
    {
		feature_vector[i]=0;
    }

    // loop over every document
    for(int i=0;i<number_documents;i++)
    {
        for(int j=0;j<documents_size[i];j++)
        {
            // check if word can be used as feature word
            if(!check_if_feature(data_list[i][j]))
			{
				position = -1;
			}
			else
			{
				// calculate index
				hashIndex = SuperHash::create_hash(data_list[i][j], (int)data_list[i][j].length(),HASH_TABLE_SIZE);
				// get the position of the word int he vocablist
				position = hash_list[hashIndex].getPositionValue(data_list[i][j]);
			}

			feature_vector[documents_indexes[i] + j] = position;
            
			//printf("%i ",feature_vector[get_document_index(documents_size,i) + j]);
        }
		//printf("\n");
		//printf("\n");
        
    }

	
	/***** DEALLOCATE UNUSED ARRAYS *****/
	//for(int i = 0;i<HASH_TABLE_SIZE;i++)
	//{
	//	hash_list[i].Destroy();
	//}

	//delete[] hash_list;

    printf("Ended Feature Construction\n");
    
}

__device__ uint32_t hash_inc(const char * data, int len, uint32_t hash)
{
    uint32_t tmp;
    int rem;
    
    if (len <= 0 || data == NULL) return 0;
    
    rem = len & 3;
    len >>= 2;
    
    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }
    
    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
            hash ^= hash << 16;
            hash ^= data[sizeof (uint16_t)] << 18;
            hash += hash >> 11;
            break;
        case 2: hash += get16bits (data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1: hash += *data;
            hash ^= hash << 10;
            hash += hash >> 1;
    }
    
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    
	return hash% HASH_TABLE_SIZE;
} 

__device__ int compare(char* a, char* b, int length_a)
{
  for(int i=0;i<length_a;i++)
  {
	  if(a[i]!=b[i])
		return 0;
  }
	  if(b[length_a]=='&')
		 return 1;
	  else
	  return 0;
}

__device__ int atomic_add(int *address, int value)
{
	int result=atomicAdd(address,value);
	return result;
}

__global__ void extract_vocab_kernel(dataCollection d_data  )
{
	int threadId = threadIdx.x + blockIdx.x*blockDim.x;
	char *word;
	int startIndex, wordLength, hashValue;
	bool wordFound=false;

	
	
		// get the start index of the current word
	startIndex=d_data.d_indexes_string_start[threadId];
	word= &d_data.d_charDataList[startIndex];
		//check if the current word is not the last word
	if((threadId+1)< d_data.total_word_count)
	{
		// get the word length = start of next word - start of current word
		wordLength= d_data.d_indexes_string_start[threadId+1]- d_data.d_indexes_string_start[threadId];
	}
	else
	{
		// case the word is the last word:get word length = total number of chars - start of current word
		wordLength= d_data.total_char_count- d_data.d_indexes_string_start[threadId];
	}


	// get the hash value of the current word
	hashValue= hash_inc(word,wordLength,(uint32_t) wordLength);
	
	//int oldNumOfWords1=  atomic_add(&d_data.d_words_per_hash_row[hashValue],1);

	for(int j=0;j< d_data.d_words_per_hash_row[hashValue];j++)
	{
		int wordStartinHashTable=hashValue*d_data.hash_row_size*d_data.hash_word_size+ j*d_data.hash_word_size; 
		if(compare(&d_data.d_charDataList[startIndex],&d_data.d_HashArray[wordStartinHashTable], wordLength))
		{
			wordFound= true;
			break;
		}
	}

	
	//int oldNumOfWords1=  atomic_add(&d_data.d_words_per_hash_row[hashValue],1);
	if(!wordFound)
	{
		// update the number of words in this hash row
		int oldNumOfWords=  atomic_add(&d_data.d_words_per_hash_row[hashValue],1);
		// caluclat the  new index where the word will be written into 
		int hashArrayIndex=hashValue*d_data.hash_row_size*d_data.hash_word_size+ oldNumOfWords*d_data.hash_word_size;
		//int hashArrayIndex=0;
		// copy data from data list to hash value
		for(int k=0;k<wordLength;k++)
		{
			d_data.d_HashArray[hashArrayIndex+k]= d_data.d_charDataList[startIndex+k];
		}
		d_data.d_HashArray[hashArrayIndex+wordLength]='&';
		//cudaMemcpy(&d_data.d_HashArray[hashArrayIndex],&d_data.d_charDataList[startIndex],wordLength*sizeof(char),cudaMemcpyDeviceToDevice);
	}


	

}

dataCollection FeatureConstructor::extract_vocab(string** data_list,int* documents_size, int number_documents, int total_word_count, int total_char_count)
{

     //loop over all documents and extract all labels
	
    printf("Began Labels Extraction\n");

	extract_labels(data_list,number_documents);
	
    printf("End of  Labels Extraction\n");

    printf("Began Vocab Extraction\n");

	   //convert the datalist to char list and extract both indexes
    //convert 2D array to 1D array of strings
	string* h_dl = CudaStd::convert_2d_to_1d_string(data_list, number_documents,documents_size,total_word_count);
	// get start index of each word
    int* indexes_string_start_data=(int*)malloc(sizeof(int)*total_word_count);//indexes for the start and length of chars
    //convert 1D array of strings to 1D array of chars
	char* h_charDataList = CudaStd::convert_string_arr_to_char_arr(h_dl,total_word_count,total_char_count,indexes_string_start_data);

	char* h_hash_array=(char*) malloc(sizeof(char)*HASH_TABLE_SIZE*HASH_ROW_SIZE*HASH_WORD_SIZE);
	int* h_words_per_hash_row= (int*)malloc(sizeof(int)*HASH_TABLE_SIZE);
	for(int i=0;i<HASH_TABLE_SIZE;i++)
	{
		h_words_per_hash_row[i]=0;
	}
	


	char *d_hash_array, *d_charDataList;
	int *d_words_per_hash_row, *d_indexes_string_start_data  ;
	
	//allocate memory of th char data list
	cudaMalloc((void**)&d_charDataList,sizeof(char)*total_char_count);
	cudaMalloc((void**)&d_indexes_string_start_data,sizeof(int)*total_word_count);
	cudaMalloc((void**)&d_hash_array,sizeof(char)*HASH_TABLE_SIZE*HASH_ROW_SIZE*HASH_WORD_SIZE);
	cudaMalloc((void**)&d_words_per_hash_row,sizeof(int)*HASH_TABLE_SIZE);

	
	cudaMemcpy(d_charDataList,h_charDataList,total_char_count*sizeof(char),cudaMemcpyHostToDevice);
	cudaMemcpy(d_indexes_string_start_data,indexes_string_start_data,total_word_count*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_words_per_hash_row,h_words_per_hash_row,HASH_TABLE_SIZE*sizeof(int),cudaMemcpyHostToDevice);
    
	
    const int THREADS_X = 512;
    const int THREADS_Y = 1;
    const int BLOCKS_X = ceil(total_word_count/(float)THREADS_X);
    const int BLOCKS_Y = 1;

	dim3 blocks(BLOCKS_X,BLOCKS_Y);
	dim3 threads(THREADS_X,THREADS_Y);

	dataCollection d_data;
	d_data.d_charDataList= d_charDataList;
	d_data.d_indexes_string_start= d_indexes_string_start_data;
	d_data.d_HashArray= d_hash_array;
	d_data.d_words_per_hash_row= d_words_per_hash_row;
	d_data.total_char_count= total_char_count;
	d_data.total_word_count= total_word_count;
	d_data.hash_table_size= HASH_TABLE_SIZE;
	d_data.hash_row_size= HASH_ROW_SIZE;
	d_data.hash_word_size= HASH_WORD_SIZE;

	extract_vocab_kernel<<<blocks,threads>>>(d_data);
	//cudaMemcpy(h_hash_array,d_hash_array,HASH_TABLE_SIZE*HASH_ROW_SIZE*HASH_WORD_SIZE*sizeof(char),cudaMemcpyDeviceToHost);
	cudaMemcpy(h_words_per_hash_row,d_words_per_hash_row,sizeof(int)*HASH_TABLE_SIZE,cudaMemcpyDeviceToHost);
 
	for(int i=0;i<HASH_TABLE_SIZE;i++)
	{
		num_unique_words+= h_words_per_hash_row[i];
	}

	free(h_charDataList);
	free(h_words_per_hash_row);
	free(h_hash_array);
	free(indexes_string_start_data);
	//delete(h_dl);

    printf("End of Vocab Extraction\n");

	return d_data;
}



#ifdef CUDA_FEATURE_VECTOR


__device__ int calculate_hash(char* word, int length)
{
	return 0;
}
/*__device__ int check_feature(char* word)
{
	bool check = true;
    if( compare(word,"though")==0 ||compare(word,"they")==0 || compare(word,"that")==0 || compare(word,"this")==0 || compare(word,"there")==0 || 
            compare(word,"were")==0 || compare(word,"than")==0 || compare(word,"rather")==0 || compare(word,"from")==0 || compare(word,"most")==0 )
        check = false;
    
    return check;
}*/
__global__ void construct_feature_vector_kernel(int* d_fv, char* d_dl, char* d_vl, int* start_index_data,int* d_table_indexes,int* start_index_vocab, int* d_positions)
{
	int threadId = threadIdx.x + blockIdx.x*blockDim.x;
	int my_word_start=start_index_data[threadId];
	char* my_word=&d_dl[my_word_start];
	int my_word_length=start_index_data[threadId+1]-start_index_data[threadId];
	int index_list=calculate_hash(my_word,my_word_length);
	int vocab_list_index=d_table_indexes[index_list];
	int vocab_list_length=d_table_indexes[index_list+1];

	for(int i=vocab_list_index;i<vocab_list_length;i=+start_index_vocab[i+1]-start_index_vocab[i])
	{
		char* vocab=&d_vl[i];
		int vocab_length=start_index_vocab[i+1]-start_index_vocab[i];
		if(compare(my_word,vocab,my_word_length)==1)
		{
			d_fv[threadId]=d_positions[i];
			break;
		}
	}
}
#endif

#ifdef CUDA_FEATURE_VECTOR
void FeatureConstructor::construct_feature_vectors(string** data_list,int* documents_size, int number_documents,int total_char_count,int total_word_count)
{
    printf("Began Feature Construction\n");
    int hashIndex, position;
	//int totalSize = calculate_table_size(documents_size,number_documents);

    // set the number of rows to be equal number of documents
    feature_vector =  (int*)malloc(sizeof(int)*total_word_count);
    documents_labels = (int*)malloc(sizeof(int)*number_documents);

    convert_labels_integers(data_list, number_documents);
	extract_documents_indexes(documents_size, number_documents);
    
    // loop on every row and set number of columns to be equal of number of unique words
    for(int i=0;i<total_word_count;i++)
    {
		feature_vector[i]=-1;
    }
	const int THREADS_Y = 1;
	const int THREADS_X = 512;
	const int BLOCKS_X = ceil(total_word_count/(float)THREADS_X);
	const int BLOCKS_Y = 1;

	dim3 blocks(BLOCKS_X,BLOCKS_Y);
	dim3 threads(THREADS_X,THREADS_Y);

	int* d_fv;//feature vector array (output)
	char* d_cl;//char list array of the data list
	char* d_vl;//hash table array
	int* d_vl_index_start;//index of the d_vl
	int* d_table_indexes;//index of the lists in the hash table
	int* d_cl_index_start;//index of the d_cl
	int* d_positions;//positions to be put in FV

	//******TEMP FOR VOCAB COMPUTATION
	int vocab_list_total_char_count=0;
	for(int i=0;i<num_unique_words;i++)
	{
		vocab_list_total_char_count+= vocab_list[i].length();
	}
	//**************
	//************TEMP FOR HASHTABLE
	int* temp_positions;
	int* temp_hashLists;
	//**************


	//allocate feature vector, char list(data list->chars), vocab list->char,index for vocab list and data list
	cudaMalloc((void**)&d_fv,total_word_count*sizeof(int));

	cudaMalloc((void**)&d_cl,total_char_count*sizeof(char));
	cudaMalloc((void**)&d_vl,vocab_list_total_char_count*sizeof(char));

	cudaMalloc((void**)&d_vl_index_start,num_unique_words*sizeof(int)+sizeof(int));
	cudaMalloc((void**)&d_table_indexes,num_unique_words*sizeof(int));//****TO DO get the size

	cudaMalloc((void**)&d_cl_index_start,total_word_count*sizeof(int)+sizeof(int));
	cudaMalloc((void**)&d_positions,num_unique_words*sizeof(int));


	//convert the datalist to char list and extract both indexes
	string* h_dl;//temp to hold the data into 1D string array
	//convert 2D array to 1D array of strings
	h_dl = CudaStd::convert_2d_to_1d_string(data_list, number_documents,documents_size,total_word_count);
	int* indexes_string_start_data=(int*)malloc(sizeof(int)*total_word_count);//indexes for the start and length of chars
	//int* indexes_string_length_data=(int*)malloc(sizeof(int)*total_word_count);
	//convert 1D array of strings to 1D array of chars
	char* h_cl = CudaStd::convert_string_arr_to_char_arr(h_dl,total_word_count,total_char_count,indexes_string_start_data);

	
	//processing the vocab list
	int* indexes_string_start_vocab=(int*)malloc(sizeof(int)*num_unique_words);//indexes for the start and length of chars
	//int* indexes_string_length_vocab=(int*)malloc(sizeof(int)*num_unique_words);
	//convert 1D array of strings to 1D array of chars
	char* h_vl = CudaStd::convert_string_arr_to_char_arr(vocab_list,num_unique_words,vocab_list_total_char_count,indexes_string_start_vocab);

	//Add the last index in the indexes lists
	d_cl_index_start[total_word_count]=total_char_count;
	d_vl_index_start[num_unique_words]=vocab_list_total_char_count;

	//copy Vocab list, vocab indexes, data indexes, char list
	cudaMemcpy(d_cl,h_cl,total_char_count*sizeof(char),cudaMemcpyHostToDevice);
	cudaMemcpy(d_vl,h_vl,vocab_list_total_char_count*sizeof(char),cudaMemcpyHostToDevice);

	//copying indexes lists
	cudaMemcpy(d_vl_index_start,indexes_string_start_vocab,num_unique_words*sizeof(int)+sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_table_indexes,temp_hashLists,num_unique_words*sizeof(int),cudaMemcpyHostToDevice);

	cudaMemcpy(d_cl_index_start,indexes_string_start_data,total_word_count*sizeof(int)+sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_positions,temp_positions,total_word_count*sizeof(int),cudaMemcpyHostToDevice);

	construct_feature_vector_kernel<<<blocks,threads>>>(d_fv,d_cl,d_vl,d_cl_index_start,d_table_indexes,d_vl_index_start,d_positions);

	cudaMemcpy(feature_vector,d_fv,total_word_count*sizeof(int),cudaMemcpyDeviceToHost);
#ifndef CUDA_FEATURE_VECTOR
    // loop over every document
    for(int i=0;i<number_documents;i++)
    {
        for(int j=0;j<documents_size[i];j++)
        {
            // check if word can be used as feature word
            if(!check_if_feature(data_list[i][j]))
			{
				position = -1;
			}
			else
			{
				// calculate index
				hashIndex = SuperHash::create_hash(data_list[i][j], (int)data_list[i][j].length(),HASH_TABLE_SIZE);
				// get the position of the word int he vocablist
				position = hash_list[hashIndex].getPositionValue(data_list[i][j]);
			}

			feature_vector[documents_indexes[i] + j] = position;
            
			//printf("%i ",feature_vector[get_document_index(documents_size,i) + j]);
        }
		//printf("\n");
		//printf("\n");
        
    }

	
	/***** DEALLOCATE UNUSED ARRAYS *****/
	//for(int i = 0;i<HASH_TABLE_SIZE;i++)
	//{
	//	hash_list[i].Destroy();
	//}

	//delete[] hash_list;
	#endif
    printf("Ended Feature Construction\n");
    
}
#endif