#include "NaiveBayesClassifier.h"

#define N_INF -1000000000

NaiveBayesClassifier::NaiveBayesClassifier(int number_labels,int number_unique_words)
{
	first_parameter = (long double**)malloc(sizeof(long double*)*number_labels);
	second_parameter = (long double*)malloc(sizeof(long double)*number_labels);

	for(int i = 0;i<number_labels;i++)
		first_parameter[i] = (long double*)malloc(sizeof(long double)*number_unique_words);


}

long double ** NaiveBayesClassifier::get_likelihood()
{
    return first_parameter;
}

long double *  NaiveBayesClassifier::get_prior()
{
    return second_parameter;
}

#ifdef CUDA_EMAD

__device__ int calculate_single_word_label_occurence(int * feature_vectors,size_t pitch, int word_index, int number_documents, int label)
{
	int result = 0;

	for(int i = 0;i<number_documents;i++)
	{
		if(label == ((int*)((char*)feature_vectors+i*pitch))[0])
			result += ((int*)((char*)feature_vectors+i*pitch))[word_index+1];
	}

	return result;

}

__global__ void calculate_occurences_kernel(int * d_fv, long double* d_fp ,size_t pitch_fv, size_t pitch_fp, int num_docs, int num_unique_words, int num_labels)
{
	
	int x = threadIdx.x + blockIdx.x*blockDim.x;
	int y = threadIdx.y;
	
	if(x < num_unique_words && y < num_labels)
	{
		((long double*)((char*)d_fp+y*pitch_fp))[x] = (long double)calculate_single_word_label_occurence(d_fv,pitch_fv,x,num_docs,y);
	}
}

__global__ void calculate_likelihood_kernel(long double* d_fp,long double* d_occ, size_t pitch_occ, size_t pitch_fp, int num_unique_words, int num_labels)
{
	
	int x = threadIdx.x + blockIdx.x*blockDim.x;
	int y = threadIdx.y;
	
	if(x < num_unique_words && y < num_labels)
	{
		int all_label_occ = 0;

		for(int i = 0;i < num_unique_words;i++)
			all_label_occ += ((long double*)((char*)d_occ+y*pitch_occ))[i];

		((long double*)((char*)d_fp+y*pitch_fp))[x] = (((long double*)((char*)d_fp+y*pitch_fp))[x] + 1)/
					(long double)(all_label_occ+num_unique_words);
	}
}

int* convert_2d_to_1d_int(int** src, int rows, int cols)
{
	int* dest = (int*)malloc(rows*cols*sizeof(int));

	for(int i = 0;i < rows;i++)
	{
		for(int j = 0;j < cols;j++)
		{
			dest[i*cols + j] = src[i][j];
		}

	}

	return dest;
}

long double** convert_1d_to_2d_float(long double* src, int rows, int cols)
{
	long double** dest = (long double**)malloc(rows*sizeof(long double*));

	for(int i = 0;i < rows;i++)
	{
		dest[i] = (long double*)malloc(cols*sizeof(long double));

		for(int j = 0;j < cols;j++)
		{
			dest[i][j] = src[i*cols + j];
		}

	}

	return dest;
}

void NaiveBayesClassifier::calculate_likelihood(int** feature_vectors,int number_unique_words, int number_documents, int number_labels)
{
	printf("Calculating First parameter...\n");
	const int THREADS_Y = number_labels;
	const int THREADS_X = 32;
	const int BLOCKS_X = ceil(number_unique_words/(float)THREADS_X);
	const int BLOCKS_Y = ceil(number_labels/(float)THREADS_Y);

	dim3 blocks(BLOCKS_X,BLOCKS_Y);
	dim3 threads(THREADS_X,THREADS_Y);

	int* d_fv, *h_fv;
	long double * d_fp, *h_fp;
	long double * d_occurences;

	size_t pitch_fv,pitch_fp,pitch_occ;

	h_fv = convert_2d_to_1d_int(feature_vectors,number_documents,number_unique_words+1);
	h_fp = (long double*)malloc(sizeof(long double)*number_unique_words*number_labels);
	
	cudaMallocPitch(&d_occurences,&pitch_occ,number_unique_words*sizeof(long double),number_labels);
	cudaMallocPitch(&d_fv,&pitch_fv,(number_unique_words+1)*sizeof(int),number_documents);
	cudaMallocPitch(&d_fp,&pitch_fp,number_unique_words*sizeof(long double),number_labels);


	cudaMemcpy2D(d_fv,pitch_fv,h_fv,(number_unique_words+1)*sizeof(int),(number_unique_words+1)*sizeof(int),number_documents,cudaMemcpyHostToDevice);
	calculate_occurences_kernel<<<blocks,threads>>>(d_fv,d_fp,pitch_fv,pitch_fp,number_documents,number_unique_words,number_labels);
	cudaMemcpy2D(d_occurences,pitch_occ,d_fp,pitch_fp,number_unique_words*sizeof(long double),number_labels,cudaMemcpyDeviceToDevice);
	calculate_likelihood_kernel<<<blocks,threads>>>(d_fp,d_occurences,pitch_occ,pitch_fp,number_unique_words,number_labels);

	cudaMemcpy2D(h_fp,number_unique_words*sizeof(long double),d_fp,pitch_fp,number_unique_words*sizeof(long double),number_labels,cudaMemcpyDeviceToHost);

	first_parameter = convert_1d_to_2d_float(h_fp,number_labels,number_unique_words);

	free(h_fp);
	free(h_fv);
	

}

#else

void NaiveBayesClassifier::calculate_likelihood(int ** feature_vectors,int number_unique_words, int number_documents, int number_labels)
{
    printf("Calculating First parameter: \n");
	for(int i = 0;i < number_labels;i++)
	{
		int all_label_occ = calculate_all_words_label_occurence(feature_vectors,number_unique_words,number_documents,i);
		for(int j = 0; j < number_unique_words;j++)
		{
			first_parameter[i][j] = (long double)(calculate_single_word_label_occurence(feature_vectors,j,number_documents,i) + 1)/
				(long double)(all_label_occ+number_unique_words);

			//printf("%f ",first_parameter[i][j]);
		}
		//printf("\n");
	}
    
    //printf("\n");
}

int NaiveBayesClassifier::calculate_single_word_label_occurence(int ** feature_vectors, int word_index, int number_documents, int label)
{
	int result = 0;

	for(int i = 0;i<number_documents;i++)
	{
		if(label == feature_vectors[i][0])
			result += feature_vectors[i][word_index+1];
	}

	return result;

}

int NaiveBayesClassifier::calculate_all_words_label_occurence(int ** feature_vectors, int number_unique_words, int number_documents, int label)
{
	int result = 0;

	for(int i = 0;i<number_unique_words;i++)
	{
		result += calculate_single_word_label_occurence(feature_vectors,i,number_documents,label);
	}

	return result;
}

#endif



void NaiveBayesClassifier::calculate_prior(int** feature_vectors, int number_documents, int number_labels)
{
    printf("Calculating Second Parameter: \n");
	int denom = number_documents+number_labels;
    
	for(int i = 0;i<number_labels;i++)
	{
		second_parameter[i] = (long double)(1+calculate_label_occurance(feature_vectors,number_documents,i))/
			(long double)denom;
        //printf("%lf \n",second_parameter[i]);
	}
    
    //printf("\n");

}

int NaiveBayesClassifier::calculate_label_occurance(int ** feature_vectors, int number_documents, int label)
{
	int result = 0;

	for(int i = 0;i < number_documents; i++)
	{
		if(feature_vectors[i][0] == label)
			result++;
	}

	return result;
}

int NaiveBayesClassifier::classify_unlabeled_document(int * unlabeled_feature_vector, int number_unique_words, int number_labels)
{
	long double max_prob = N_INF;
	int label = -1;
    
	for(int i = 0;i < number_labels;i++)
	{
		long double prob = (long double)NaiveBayesClassifier::prob_document_label(unlabeled_feature_vector,number_unique_words,i);

        if(prob > max_prob)
		{
			max_prob = prob;
			label = i;
		}
	}

	//unlabeled_feature_vector[0] = label;
	return label;
}

long double NaiveBayesClassifier::prob_document_all_labels(int * feature_vector, int number_unique_words, int number_labels)
{
	long double result = 0;

	for(int i = 0;i<number_labels;i++)
	{
		result += NaiveBayesClassifier::prob_document_label(feature_vector,number_unique_words,i);
	}

	return result;
}


long double NaiveBayesClassifier::prob_document_label(int * feature_vector, int number_unique_words, int label)
{
	long double result = log10(second_parameter[label]);

	//printf("Label: %d Prior: %Lf \n",label,result);

	for(int i = 0;i<number_unique_words;i++)
	{
        if(first_parameter[label][i] == 0)
            continue;
		result = result + (feature_vector[i+1]*log10(first_parameter[label][i]));
		//printf("result:  %f %d = %e \n",first_parameter[label][i], feature_vector[i] ,result);
	}

    //printf("Label: %d Final Prob: %Lf\n",label,result);
	return result;
}

