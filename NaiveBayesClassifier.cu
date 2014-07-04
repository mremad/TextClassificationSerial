#include "NaiveBayesClassifier.h"
#include "ConsolePrint.h"

#define N_INF -1000000000

NaiveBayesClassifier::NaiveBayesClassifier(int number_labels,int number_unique_words)
{
	first_parameter = (long double**)malloc(sizeof(long double*)*number_labels);
	second_parameter = (long double*)malloc(sizeof(long double)*number_labels);

	for(int i = 0;i<number_labels;i++)
		first_parameter[i] = (long double*)malloc(sizeof(long double)*number_unique_words);


}

#ifdef CUDA_LIKELIHOOD

__device__ int calculate_single_word_label_occurence(int * feature_vectors,int * documents_size, int * documents_indexes, int * docs_labels, int word_index, int number_documents, int label)
{
	int result = 0;

	for(int i = 0;i<number_documents;i++)
	{
		if(label == docs_labels[i])
		{
			for(int j = 0;j < documents_size[i];j++)
			{
				if(feature_vectors[documents_indexes[i] + j] == word_index)
					result++;
			}

		}
			
	}


	return result;

}

__global__ void calculate_occurences_kernel(int * d_fv, int* d_fp ,int * documents_size, int * documents_indexes, int * docs_labels, size_t pitch_fp, int num_docs, int num_unique_words, int num_labels)
{
	
	int x = threadIdx.x + blockIdx.x*blockDim.x;
	int y = threadIdx.y;
	
	if(x < num_unique_words && y < num_labels)
	{
		((int*)((char*)d_fp+y*pitch_fp))[x] = calculate_single_word_label_occurence(d_fv,documents_size,documents_indexes,docs_labels,x,num_docs,y);
	}
}

__global__ void calculate_likelihood_kernel(long double* d_fp,int* d_occ, size_t pitch_occ, size_t pitch_fp, int num_unique_words, int num_labels)
{
	
	int x = threadIdx.x + blockIdx.x*blockDim.x;
	int y = threadIdx.y;
	
	if(x < num_unique_words && y < num_labels)
	{
		int all_label_occ = 0;

		for(int i = 0;i < num_unique_words;i++)
			all_label_occ += ((int*)((char*)d_occ+y*pitch_occ))[i];

		((long double*)((char*)d_fp+y*pitch_fp))[x] = (((int*)((char*)d_occ+y*pitch_occ))[x] + 1)/
					(long double)(all_label_occ+num_unique_words);
	}
}

void NaiveBayesClassifier::calculate_likelihood(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int number_unique_words, int number_documents, int number_labels)
{
	printf("Calculating First parameter...\n");
	const int THREADS_Y = number_labels;
	const int THREADS_X = LIKELIHOOD_THREADS_X;
	const int BLOCKS_X = ceil(number_unique_words/(float)THREADS_X);
	const int BLOCKS_Y = ceil(number_labels/(float)THREADS_Y);
	const int FV_SIZE = (documents_indexes[number_documents-1] + documents_size[number_documents - 1]);

	dim3 blocks(BLOCKS_X,BLOCKS_Y);
	dim3 threads(THREADS_X,THREADS_Y);

	int* d_fv, *d_doc_size, *d_doc_ind, *d_doc_label;
	long double * d_fp, *h_fp;
	int * d_occurences;

	size_t pitch_fp,pitch_occ;

	h_fp = (long double*)malloc(sizeof(long double)*number_unique_words*number_labels);
	
	cudaMallocPitch(&d_occurences,&pitch_occ,number_unique_words*sizeof(int),number_labels);
	cudaMalloc((void**)&d_fv,FV_SIZE*sizeof(int));
	cudaMalloc((void**)&d_doc_size,number_documents*sizeof(int));
	cudaMalloc((void**)&d_doc_label,number_documents*sizeof(int));
	cudaMalloc((void**)&d_doc_ind,number_documents*sizeof(int));
	cudaMallocPitch(&d_fp,&pitch_fp,number_unique_words*sizeof(long double),number_labels);

	cudaMemcpy(d_fv,feature_vectors,FV_SIZE*sizeof(int),cudaMemcpyHostToDevice);

	cudaMemcpy(d_doc_size,documents_size,number_documents*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_doc_label,docs_labels,number_documents*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_doc_ind,documents_indexes,number_documents*sizeof(int),cudaMemcpyHostToDevice);

	calculate_occurences_kernel<<<blocks,threads>>>(d_fv,d_occurences,d_doc_size,d_doc_ind,d_doc_label,pitch_occ,number_documents,number_unique_words,number_labels);
	calculate_likelihood_kernel<<<blocks,threads>>>(d_fp,d_occurences,pitch_occ,pitch_fp,number_unique_words,number_labels);

	cudaMemcpy2D(h_fp,number_unique_words*sizeof(long double),d_fp,pitch_fp,number_unique_words*sizeof(long double),number_labels,cudaMemcpyDeviceToHost);

	first_parameter = CudaStd::convert_1d_to_2d_float(h_fp,number_labels,number_unique_words);

	free(h_fp);

	cudaFree(d_fv);
	cudaFree(d_doc_label);
	cudaFree(d_doc_size);
	cudaFree(d_doc_ind);
	cudaFree(d_fp);
	cudaFree(d_occurences);
}

#else

void NaiveBayesClassifier::calculate_likelihood(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int number_unique_words, int number_documents, int number_labels)
{
    printf("Calculating First parameter: \n");
	for(int i = 0;i < number_labels;i++)
	{
		int all_label_occ = calculate_all_words_label_occurence(feature_vectors, documents_size, documents_indexes, docs_labels,number_unique_words,number_documents,i);
		for(int j = 0; j < number_unique_words;j++)
		{
			first_parameter[i][j] = (long double)(calculate_single_word_label_occurence(feature_vectors, documents_size, documents_indexes, docs_labels ,j,number_documents,i) + 1)/
				(long double)(all_label_occ+number_unique_words);

			if(j<30)
			printf("%d ",calculate_single_word_label_occurence(feature_vectors, documents_size, documents_indexes, docs_labels ,j,number_documents,i));
		}
		printf("\n");
	}
    
    printf("\n");
}

int NaiveBayesClassifier::calculate_single_word_label_occurence(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int word_index, int number_documents, int label)
{
	int result = 0;

	for(int i = 0;i<number_documents;i++)
	{
		if(label == docs_labels[i])
		{
			for(int j = 0;j < documents_size[i];j++)
			{
				if(feature_vectors[documents_indexes[i] + j] == word_index)
					result++;
			}

		}
			
	}

	return result;

}

int NaiveBayesClassifier::calculate_all_words_label_occurence(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int number_unique_words, int number_documents, int label)
{
	int result = 0;

	for(int i = 0;i<number_unique_words;i++)
	{
		result += calculate_single_word_label_occurence(feature_vectors, documents_size, documents_indexes, docs_labels,i,number_documents,label);
	}

	return result;
}

#endif

#ifndef CUDA_CLASSIFY

int NaiveBayesClassifier::classify_unlabeled_document(int * unlabeled_feature_vector, int fv_size, int number_unique_words, int number_labels)
{
	long double max_prob = N_INF;
	int label = -1;
    
	for(int i = 0;i < number_labels;i++)
	{
		long double prob = (long double)NaiveBayesClassifier::prob_document_label(unlabeled_feature_vector, fv_size ,number_unique_words,i);

        if(prob > max_prob)
		{
			max_prob = prob;
			label = i;
		}
	}

	return label;
}

long double NaiveBayesClassifier::prob_document_label(int * feature_vector, int fv_size, int number_unique_words, int label)
{
	long double result = log10(second_parameter[label]);

	//printf("Label: %d Prior: %Lf \n",label,result);

	for(int i = 0;i<fv_size;i++)
	{
        if(feature_vector[i] == -1 || first_parameter[label][feature_vector[i]] == 0)
            continue;
		result = result + log10(first_parameter[label][feature_vector[i]]);
		//printf("result:  %f %d = %e \n",first_parameter[label][i], feature_vector[i] ,result);
	}

    //printf("Label: %d Final Prob: %Lf\n",label,result);
	return result;
}

#else


__device__ long double prob_document_label(int * feature_vector,long double* fp, long double*sp, int fv_size, int number_unique_words, int label)
{
	
	long double result = log10((double)sp[label]);
	//printf("Label: %d Prior: %Lf \n",label,result);

	for(int i = 0;i<fv_size;i++)
	{
        if(feature_vector[i] == -1 || (fp+label*number_unique_words)[feature_vector[i]] == 0)
            continue;
		result = result + log10((double)(fp+label*number_unique_words)[feature_vector[i]]);
		//printf("result:  %f %d = %e \n",first_parameter[label][i], feature_vector[i] ,result);
	}

    //printf("Label: %d Final Prob: %Lf\n",label,result);
	return result;
}

__device__ int classify_unlabeled_document(int * unlabeled_feature_vector,long double* fp, long double* sp, int fv_size, int number_unique_words, int number_labels)
{
	long double max_prob = N_INF;
	int label = -1;
    
	for(int i = 0;i < number_labels;i++)
	{
		long double prob = (long double)prob_document_label(unlabeled_feature_vector, fp, sp, fv_size ,number_unique_words,i);

        if(prob > max_prob)
		{
			max_prob = prob;
			label = i;
		}
	}

	return label;
}

__global__ void classify_unlabeled_docs_kernel(int * unlabeled_feature_vectors, int* doc_sizes, int* docs_indexes, int num_docs ,long double* fp, long double* sp, int number_unique_words, int number_labels, int* docs_labels)
{
	int x = threadIdx.x + blockIdx.x*blockDim.x;

	if(x < num_docs)
	{
		docs_labels[x] = classify_unlabeled_document(unlabeled_feature_vectors+docs_indexes[x],fp,sp,doc_sizes[x],number_unique_words,number_labels);
	}
}




#endif

void NaiveBayesClassifier::classify_unlabeled_documents(int* feature_vectors, int* docs_sizes, int* docs_indexes, int num_documents, int num_unique_words, int num_labels, int* labels)
{
#ifndef CUDA_CLASSIFY
	for(int i = 0;i < num_documents;i++)
	{
		labels[i] = classify_unlabeled_document(feature_vectors+docs_indexes[i],docs_sizes[i],num_unique_words,num_labels);
	}
#else
	const int THREADS_Y = CLASSIFY_THREADS_Y;
	const int THREADS_X = CLASSIFY_THREADS_X;
	const int BLOCKS_X = ceil(num_documents/(float)THREADS_X);
	const int BLOCKS_Y = ceil(1/(float)THREADS_Y);
	const int FV_SIZE = (docs_indexes[num_documents-1] + docs_sizes[num_documents - 1]);

	dim3 blocks(BLOCKS_X,BLOCKS_Y);
	dim3 threads(THREADS_X,THREADS_Y);

	int* d_fv, *d_docs_sizes, *d_docs_indexes, *d_labels;
	long double* d_fp, *d_sp;
	long double *h_fp;

	cudaMalloc((void**)&d_fv,FV_SIZE*sizeof(int));
	cudaMalloc((void**)&d_docs_sizes,num_documents*sizeof(int));
	cudaMalloc((void**)&d_docs_indexes,num_documents*sizeof(int));
	cudaMalloc((void**)&d_labels,num_documents*sizeof(int));
	cudaMalloc((void**)&d_fp,num_unique_words*num_labels*sizeof(long double));
	cudaMalloc((void**)&d_sp,num_labels*sizeof(long double));

	h_fp = CudaStd::convert_2d_to_1d_float(first_parameter,num_labels,num_unique_words);

	cudaMemcpy(d_fv,feature_vectors,FV_SIZE*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_docs_sizes,docs_sizes,num_documents*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_docs_indexes,docs_indexes,num_documents*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_fp,h_fp,num_unique_words*num_labels*sizeof(long double),cudaMemcpyHostToDevice);
	cudaMemcpy(d_sp,second_parameter,num_labels*sizeof(long double),cudaMemcpyHostToDevice);

	classify_unlabeled_docs_kernel<<<blocks,threads>>>(d_fv,d_docs_sizes,d_docs_indexes,num_documents,d_fp,d_sp,num_unique_words,num_labels,d_labels);

	cudaMemcpy(labels,d_labels,num_documents*sizeof(int),cudaMemcpyDeviceToHost);

	free(h_fp);

	cudaFree(d_fv);
	cudaFree(d_docs_sizes);
	cudaFree(d_docs_indexes);
	cudaFree(d_labels);
	cudaFree(d_fp);
	cudaFree(d_sp);

#endif
}

int NaiveBayesClassifier::calculate_label_occurance(int * documents_labels, int number_documents, int label)
{
	int result = 0;

	for(int i = 0;i < number_documents; i++)
	{
		if(documents_labels[i] == label)
			result++;
	}

	return result;
}

void NaiveBayesClassifier::calculate_prior(int* documents_labels, int number_documents, int number_labels)
{
    printf("Calculating Second Parameter: \n");
	int denom = number_documents+number_labels;
    
	for(int i = 0;i<number_labels;i++)
	{
		second_parameter[i] = (long double)(1+calculate_label_occurance(documents_labels,number_documents,i))/
			(long double)denom;
        //printf("%lf \n",second_parameter[i]);
	}
    
    //printf("\n");

}

long double ** NaiveBayesClassifier::get_likelihood()
{
    return first_parameter;
}

long double *  NaiveBayesClassifier::get_prior()
{
    return second_parameter;
}