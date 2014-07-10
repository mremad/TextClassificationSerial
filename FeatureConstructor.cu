#include "FeatureConstructor.h"


using namespace std;

dataCollection dataCollection_new()
{
	 dataCollection a;

  return a;
}

FeatureConstructor::FeatureConstructor()
{
    
}

void FeatureConstructor::process_data_list(string** data_list,int* documents_size, int number_documents)
{
	total_char_count=0;
	total_word_count=0;
	for(int i=0;i<number_documents;i++)
	{
		total_word_count+=documents_size[i];
		for(int j=0;j<documents_size[i];j++)
		{
			total_char_count+=data_list[i][j].length();
		}
	}

	
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
	
    label_list= new string[MAX_NUM_LABELS];
    num_labels=0;
	num_unique_words=0;
	documents_size = document_size;
    label_list= new string[MAX_NUM_LABELS];
    hash_list= new LinkedList[HASH_TABLE_SIZE];
    max_List_Size=0;
	for(int i=0;i<HASH_TABLE_SIZE;i++)
    {
        hash_list[i]= *new LinkedList();
    }

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
        for(int j=0;j<documents_size[i];j++)
        {
            // calculate the hash index
            int hashIndex= SuperHash::create_hash(data_list[i][j], (int)data_list[i][j].length(),HASH_TABLE_SIZE);
            // if the word wasn't already added to the list in the specified index
            if(!hash_list[hashIndex].Exists(data_list[i][j]))
            {
                // append word to the list
                hash_list[hashIndex].Append((data_list[i][j]),num_unique_words);

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

__device__ int hash_str(const char* s, int str_length) 
{ 
unsigned h = 31 /* also prime */; 

for(int i = 0; i < str_length;i++) 
{ 
h = (h * 54059 ) ^ (s[0] * 76963); 
} 

return h % HASH_TABLE_SIZE; 
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

// kernel for feature construction
__global__ void construct_feature_vector_kernel (int* d_fv, dataCollection d_data)
{
	//initialize threadId and a word from the data list for each thread
	int threadId = threadIdx.x + blockIdx.x*blockDim.x;
	if(threadId<d_data.total_word_count)
	{	
		d_fv[threadId]=-1;
		//each word has its starting index in the 1D char array saved in the data_start_indexes
		int my_word_start=d_data.d_indexes_string_start[threadId];

		//char* my_word=&d_data.char_data_list [my_word_start];
	
		int my_word_length;
		if(threadId+1<d_data.total_word_count)
			my_word_length=d_data.d_indexes_string_start[threadId+1]-d_data.d_indexes_string_start[threadId];
		else
			my_word_length=d_data.total_char_count-d_data.d_indexes_string_start[threadId];
		//calculate the hash function for this word and get the range in the vocab array to search in
		int index_list=hash_str(&d_data.d_charDataList [my_word_start],my_word_length);
		int vocab_list_index=index_list*d_data.hash_row_size*d_data.hash_word_size;
		int vocab_list_length=d_data.d_words_per_hash_row[index_list]*d_data.hash_word_size;

		//loop over the range from the vocab list and if the word matches a word stored in the vocab list save its index
		for(int i=vocab_list_index;i<(vocab_list_length+vocab_list_index) ;i+=d_data.hash_word_size)
		{
			if(compare(&d_data.d_charDataList [my_word_start],&d_data.d_HashArray[i],my_word_length)==1)
			{
				d_fv[threadId]=d_data.d_word_index_in_vocab[i];
				break;
			}
		}
	}
	
}

void FeatureConstructor::construct_feature_vectors(string** data_list,int* documents_size, int number_documents)
{
    
    printf("Began Feature Construction\n");
#ifndef CUDA_FEATURE_VECTOR
    int hashIndex, position;
	//int totalSize = calculate_table_size(documents_size,number_documents);

#endif
    feature_vector =  (int*)malloc(sizeof(int)*total_word_count);
    documents_labels = (int*)malloc(sizeof(int)*number_documents);

    convert_labels_integers(data_list, number_documents);
	extract_documents_indexes(documents_size, number_documents);
    

#ifndef CUDA_FEATURE_VECTOR
	// loop on every row and set number of columns to be equal of number of unique words
    for(int i=0;i<total_word_count;i++)
    {
		feature_vector[i]=0;
    }

    // loop over every document
    for(int i=0;i<number_documents;i++)
    {
        for(int j=0;j<documents_size[i];j++)
        {
				// calculate index
				hashIndex = SuperHash::create_hash(data_list[i][j], (int)data_list[i][j].length(),HASH_TABLE_SIZE);
				// get the position of the word int he vocablist
				position = hash_list[hashIndex].getPositionValue(data_list[i][j]);

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

#ifdef CUDA_FEATURE_VECTOR
   
	const int THREADS_Y = 1;
	const int THREADS_X = 512;
	const int BLOCKS_X = ceil(total_word_count/(float)THREADS_X);
	const int BLOCKS_Y = 1;

	dim3 blocks(BLOCKS_X,BLOCKS_Y);
	dim3 threads(THREADS_X,THREADS_Y);

	int*  d_fv; //holding results
	char* d_cl;
	int* d_cl_index_start;
	char* d_vl;
	int* words_per_row;
	int* d_word_index_in_vocab;


	//allocate feature vector, char list(data list->chars), vocab list->char,index for vocab list and data list
	cudaMalloc((void**)&d_fv,total_word_count*sizeof(int));

	cudaMalloc((void**)&d_word_index_in_vocab,HASH_ROW_SIZE*HASH_WORD_SIZE*HASH_TABLE_SIZE*sizeof(int));

	cudaMalloc((void**)&d_cl,total_char_count*sizeof(char));
	cudaMalloc((void**)&d_vl,HASH_ROW_SIZE*HASH_WORD_SIZE*HASH_TABLE_SIZE*sizeof(char));

	cudaMalloc((void**)&words_per_row,HASH_TABLE_SIZE*sizeof(int));

	cudaMalloc((void**)&d_cl_index_start,total_word_count*sizeof(int));

	

	//convert the datalist to char list and extract both indexes
	string* h_dl;//temp to hold the data into 1D string array
	//convert 2D array to 1D array of strings
	h_dl = CudaStd::convert_2d_to_1d_string(data_list, number_documents,documents_size,total_word_count);
	int* indexes_string_start_data=(int*)malloc(sizeof(int)*total_word_count);//indexes for the start and length of data strings
	//convert 1D array of strings to 1D array of chars
	char* h_cl = CudaStd::convert_string_arr_to_char_arr(h_dl,total_word_count,total_char_count,indexes_string_start_data);

	//constants for the data structure
	dataCollection data;
	data.d_charDataList=d_cl;
	data.d_indexes_string_start=d_cl_index_start;
	data.d_HashArray=d_vl;
	data.hash_row_size=HASH_ROW_SIZE;
	data.hash_table_size=HASH_TABLE_SIZE;
	data.hash_word_size=HASH_WORD_SIZE;
	data.total_char_count=total_char_count;
	data.total_word_count=total_word_count;
	data.d_words_per_hash_row=words_per_row;
	data.d_word_index_in_vocab=d_word_index_in_vocab;

	//copy Vocab list, vocab indexes, data indexes, char list
	cudaMemcpy(d_cl,h_cl,total_char_count*sizeof(char),cudaMemcpyHostToDevice);

	cudaMemcpy(d_vl,h_hash_array,HASH_ROW_SIZE*HASH_WORD_SIZE*HASH_TABLE_SIZE*sizeof(char),cudaMemcpyHostToDevice);

	cudaMemcpy(d_word_index_in_vocab,h_word_index_in_vocab,HASH_ROW_SIZE*HASH_WORD_SIZE*HASH_TABLE_SIZE*sizeof(int),cudaMemcpyHostToDevice);

	//copying indexes lists
	cudaMemcpy(words_per_row,h_words_per_hash_row,HASH_TABLE_SIZE*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_cl_index_start,indexes_string_start_data,total_word_count*sizeof(int),cudaMemcpyHostToDevice);
	
 	construct_feature_vector_kernel<<<blocks,threads>>>(d_fv,data);
	if ( cudaSuccess != cudaGetLastError() )
		printf("\n %s \n", cudaGetErrorString(cudaGetLastError()));
    //printf( "Error! Feature constructor parallel\n" );

	cudaMemcpy(feature_vector,d_fv,total_word_count*sizeof(int),cudaMemcpyDeviceToHost);
	cudaFree(d_cl);
	cudaFree(d_vl);
	cudaFree(words_per_row);
	cudaFree(d_fv);
	cudaFree(d_cl_index_start);
	cudaFree(d_word_index_in_vocab);
	free(h_cl);
	free(indexes_string_start_data);

    printf("Ended Feature Construction\n");
   
#endif 
}

__device__ int atomic_add(int *address, int value)
{
	int result=atomicAdd(address,value);
	return result;
}

__device__ void shiftData( char* source, int data_length, int shift)
{

	// if shift is to the right, copy from the end first
	if(shift >0)
	{
		for(int i=data_length-1;i>0;i--)
		{
			source[i+shift]= source[i];
		}
	}
	else 
	{
		// if shift is to the left, copy from the start first
		for(int i=0;i<data_length;i++)
		{
			source[i+shift]= source[i];
		}

	}
	
}

__global__ void remove_duplicates_in_hashTable(dataCollection d_data)
{
	int threadId = threadIdx.x + blockIdx.x*blockDim.x;
	
	if(threadId <d_data.hash_table_size)
	{
		for(int i=0;i<d_data.d_words_per_hash_row[threadId];i++)
		{
			int wordLength=0;
			int hashArrayIndex= threadId*d_data.hash_row_size*d_data.hash_word_size+ + i*d_data.hash_word_size;
			for(int j=0;j<d_data.hash_word_size;j++)
			{
				if(d_data.d_HashArray[hashArrayIndex+j]=='&')
					break;
				else
					wordLength++;
			}

			for(int k=i+1;k< d_data.d_words_per_hash_row[threadId];k++)
			{
				int comparedWordStart= threadId*d_data.hash_row_size*d_data.hash_word_size+ + k*d_data.hash_word_size;
				if(compare(&d_data.d_HashArray[hashArrayIndex] ,&d_data.d_HashArray[comparedWordStart], wordLength))
				{
					// check if compared word is the last word
					if(k == (d_data.d_words_per_hash_row[threadId]-1))
					{
						// just decrement the number of words in hash & ignore last word
						d_data.d_words_per_hash_row[threadId]--;
					}
					else
					{	
						// shift the next cells to the left & decrement the number of words per hash
						int startOfShiftedData= comparedWordStart+d_data.hash_word_size;
						int shiftedDataLength= (d_data.d_words_per_hash_row[threadId]-1-k)*d_data.hash_word_size;
						shiftData(&d_data.d_HashArray[startOfShiftedData],shiftedDataLength,-d_data.hash_word_size);
						d_data.d_words_per_hash_row[threadId]--;
						k--;
					}
				}
			}
		}
	}
	
}

__global__ void extract_vocab_kernel(dataCollection d_data  )
{
	int threadId = threadIdx.x + blockIdx.x*blockDim.x;
	if(threadId<d_data.total_word_count)
	{
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
		hashValue= hash_str(word,wordLength);
	
		// Loop in the words in the hash table row with same hash value 
		// to see if word exists or not
		for(int j=0;j< d_data.d_words_per_hash_row[hashValue];j++)
		{	
			// calculate the start of the word in hash table
			int wordStartinHashTable=hashValue*d_data.hash_row_size*d_data.hash_word_size+ j*d_data.hash_word_size; 
			if(compare(&d_data.d_charDataList[startIndex],&d_data.d_HashArray[wordStartinHashTable], wordLength))
			{
				wordFound= true;
				break;
			}
		}

		if(!wordFound)
		{
			// update the number of words in this hash row
			int oldNumOfWords=  atomic_add(&d_data.d_words_per_hash_row[hashValue],1);
			// caluclat the  new index where the word will be written into 
			int hashArrayIndex=hashValue*d_data.hash_row_size*d_data.hash_word_size+ oldNumOfWords*d_data.hash_word_size;
		
			// copy data from data list to hash value
			for(int k=0;k<wordLength;k++)
			{
				d_data.d_HashArray[hashArrayIndex+k]= d_data.d_charDataList[startIndex+k];
			}
			d_data.d_HashArray[hashArrayIndex+wordLength]='&';
		}
	}

	
	
}

void FeatureConstructor::extract_vocab(string** data_list,int* documents_size, int number_documents, int total_word_count, int total_char_count)
{
     //loop over all documents and extract all labels
    printf("Began Labels Extraction\n");
	extract_labels(data_list,number_documents);
    printf("End of  Labels Extraction\n");

    printf("Began Vocab Extraction\n");

	// initialize variables
	int* indexes_string_start_data;
	char* h_charDataList;
	string* h_dl;
	char *d_hash_array, *d_charDataList;
	int *d_words_per_hash_row, *d_indexes_string_start_data  ;

    //convert 2D array to 1D array of strings
	 h_dl = CudaStd::convert_2d_to_1d_string(data_list, number_documents,documents_size,total_word_count);
	// get start index of each word
     indexes_string_start_data=(int*)malloc(sizeof(int)*total_word_count);//indexes for the start and length of chars
    //convert 1D array of strings to 1D array of chars
	 h_charDataList = CudaStd::convert_string_arr_to_char_arr(h_dl,total_word_count,total_char_count,indexes_string_start_data);


	 //Allocate memory to arrays on the host
	 // size of h_hash_array= (number of rows in hash table)* (each row size)* (size of each word) 
	h_hash_array=(char*) malloc(sizeof(char)*HASH_TABLE_SIZE*HASH_ROW_SIZE*HASH_WORD_SIZE);
	h_words_per_hash_row= (int*)malloc(sizeof(int)*HASH_TABLE_SIZE);

	// initialize h_words_per_hash_row with zeros
	for(int i=0;i<HASH_TABLE_SIZE;i++)
	{
		h_words_per_hash_row[i]=0;
	}
	
	//allocate memory on the device
	cudaMalloc((void**)&d_charDataList,sizeof(char)*total_char_count);
	cudaMalloc((void**)&d_indexes_string_start_data,sizeof(int)*total_word_count);
	cudaMalloc((void**)&d_hash_array,sizeof(char)*HASH_TABLE_SIZE*HASH_ROW_SIZE*HASH_WORD_SIZE);
	cudaMalloc((void**)&d_words_per_hash_row,sizeof(int)*HASH_TABLE_SIZE);

	// copy data from device to host
	cudaMemcpy(d_charDataList,h_charDataList,total_char_count*sizeof(char),cudaMemcpyHostToDevice);
	cudaMemcpy(d_indexes_string_start_data,indexes_string_start_data,total_word_count*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_words_per_hash_row,h_words_per_hash_row,HASH_TABLE_SIZE*sizeof(int),cudaMemcpyHostToDevice);
    
	
    const int THREADS_X = 512;
    const int THREADS_Y = 1;
    const int BLOCKS_X = ceil(total_word_count/(float)THREADS_X);
    const int BLOCKS_Y = 1;

	dim3 blocks(BLOCKS_X,BLOCKS_Y);
	dim3 threads(THREADS_X,THREADS_Y);

	// use the dataCollectin struct to pass arguments to the kernel
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

	// run the kernel 
	extract_vocab_kernel<<<blocks,threads>>>(d_data);
	const int numberOfBlocks= ceil(HASH_TABLE_SIZE/(float)THREADS_X);

	dim3 hash_blocks(numberOfBlocks,1);
	remove_duplicates_in_hashTable<<<hash_blocks,threads>>>(d_data);


	// copy the data back from device to host
	cudaMemcpy(h_hash_array,d_hash_array,HASH_TABLE_SIZE*HASH_ROW_SIZE*HASH_WORD_SIZE*sizeof(char),cudaMemcpyDeviceToHost);
	cudaMemcpy(h_words_per_hash_row,d_words_per_hash_row,sizeof(int)*HASH_TABLE_SIZE,cudaMemcpyDeviceToHost);
 
	// calculate numver of unique words 
	for(int i=0;i<HASH_TABLE_SIZE;i++)
	{
		num_unique_words+= h_words_per_hash_row[i];
	}

	/*for(int i=0;i<HASH_TABLE_SIZE;i++)
	{
		for(int j=0;j<h_words_per_hash_row[i];j++)
		{
			for(int k=0;k<HASH_WORD_SIZE;k++)
			{
				int index= i*HASH_ROW_SIZE*HASH_WORD_SIZE+ j*HASH_WORD_SIZE;
				if(h_hash_array[index+k]== '&')
					break;

				printf("%c",h_hash_array[index+k]);
			}
			 printf(" - ");
		}

		printf("\n End of Row %d ------------------------------------------------------ \n" ,i);
	}*/
	
	//hold index in vocab list
	// Row size = 10 words
	// word size = 10 chars
	//3 words  1st row :(0)messi  ,(10) ronaldo  (20) treka
	//2 words   2nd row ; (0) emad , (10) shaaban
	//0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0
	//
 
	
	h_word_index_in_vocab=(int*)malloc(sizeof(int)*HASH_ROW_SIZE*HASH_WORD_SIZE*HASH_TABLE_SIZE);

		for(int j=0;j<HASH_ROW_SIZE*HASH_WORD_SIZE*HASH_TABLE_SIZE;j++)
		{
			h_word_index_in_vocab[j]=-1;
		}
	//calculate index of each vocab word in the vocab list
	int index_in_h_hash_array=0;
	for(int i=0;i<HASH_TABLE_SIZE;i++)
		for(int j=0;j<h_words_per_hash_row[i];j++)
		{
			h_word_index_in_vocab[j*HASH_WORD_SIZE+i*HASH_ROW_SIZE*HASH_WORD_SIZE]=index_in_h_hash_array;
			index_in_h_hash_array++;
		}
		
	// Free memory on the device
	cudaFree(d_data.d_charDataList);
	cudaFree(d_data.d_HashArray);
	cudaFree(d_data.d_indexes_string_start);
	cudaFree(d_data.d_words_per_hash_row);

	// free memory on the host
	free(h_charDataList);
	free(indexes_string_start_data);
	//delete(h_dl);

    printf("End of Vocab Extraction\n");

}