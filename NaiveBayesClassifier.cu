#include "NaiveBayesClassifier.h"
#include "ConsolePrint.h"

#define N_INF -1000000000

NaiveBayesClassifier::NaiveBayesClassifier(int number_labels,int number_unique_words)
{
	first_parameter = (double**)malloc(sizeof(double*)*number_labels);
	second_parameter = (double*)malloc(sizeof(double)*number_labels);

	for(int i = 0;i<number_labels;i++)
		first_parameter[i] = (double*)malloc(sizeof(double)*number_unique_words);


}

#ifdef CUDA_LIKELIHOOD

__device__ double calculate_single_word_label_occurence(int * feature_vectors,int * documents_size, int * documents_indexes, int * docs_labels, int word_index, int number_documents, int label,int num_labels, double* weights)
{
	double result = 0;

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
		else if(docs_labels[i] == -1)
		{
			for(int j = 0;j < documents_size[i];j++)
			{
				if(feature_vectors[documents_indexes[i] + j] == word_index)
				{
						result += (weights[i*num_labels + label]*1);
				}
			}
		}
			
	}


	return result;

}

__global__ void calculate_occurences_kernel(int * d_fv, double* d_fp ,int * documents_size, int * documents_indexes, int * docs_labels, size_t pitch_fp, int num_docs, int num_unique_words, int num_labels, double* weights)
{
	
	int x = threadIdx.x + blockIdx.x*blockDim.x;
	int y = threadIdx.y;
	
	if(x < num_unique_words && y < num_labels)
	{
		((double*)((char*)d_fp+y*pitch_fp))[x] = calculate_single_word_label_occurence(d_fv,documents_size,documents_indexes,docs_labels,x,num_docs,y,num_labels,weights);
	}
}

__global__ void calculate_likelihood_kernel(double* d_fp,double* d_occ, size_t pitch_occ, size_t pitch_fp, int num_unique_words, int num_labels)
{
	
	int x = threadIdx.x + blockIdx.x*blockDim.x;
	int y = threadIdx.y;
	
	if(x < num_unique_words && y < num_labels)
	{
		double all_label_occ = 0;

		for(int i = 0;i < num_unique_words;i++)
			all_label_occ += ((double*)((char*)d_occ+y*pitch_occ))[i];

		((double*)((char*)d_fp+y*pitch_fp))[x] = (((double*)((char*)d_occ+y*pitch_occ))[x] + 1)/
					(double)(all_label_occ+num_unique_words);
	}
}

void NaiveBayesClassifier::calculate_likelihood(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int number_unique_words, int number_documents, int number_labels, double* weights)
{
	printf("Calculating First parameter...\n");

	this->weights = weights;
	this->num_labels = number_labels;

	const int THREADS_Y = number_labels;
	const int THREADS_X = LIKELIHOOD_THREADS_X;
	const int BLOCKS_X = ceil(number_unique_words/(float)THREADS_X);
	const int BLOCKS_Y = ceil(number_labels/(float)THREADS_Y);
	const int FV_SIZE = (documents_indexes[number_documents-1] + documents_size[number_documents - 1]);

	dim3 blocks(BLOCKS_X,BLOCKS_Y);
	dim3 threads(THREADS_X,THREADS_Y);

	int* d_fv, *d_doc_size, *d_doc_ind, *d_doc_label;
	double * d_fp, *h_fp;
	double * d_occurences;
	double*  d_weights;

	size_t pitch_fp,pitch_occ;

	h_fp = (double*)malloc(sizeof(double)*number_unique_words*number_labels);
	
	cudaMallocPitch(&d_occurences,&pitch_occ,number_unique_words*sizeof(double),number_labels);
	cudaMalloc((void**)&d_fv,FV_SIZE*sizeof(int));
	cudaMalloc((void**)&d_doc_size,number_documents*sizeof(int));
	cudaMalloc((void**)&d_doc_label,number_documents*sizeof(int));
	cudaMalloc((void**)&d_doc_ind,number_documents*sizeof(int));
	cudaMalloc((void**)&d_weights,number_documents*number_labels*sizeof(double));
	cudaMallocPitch(&d_fp,&pitch_fp,number_unique_words*sizeof(double),number_labels);

	cudaMemcpy(d_fv,feature_vectors,FV_SIZE*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_weights,weights,number_documents*number_labels*sizeof(double),cudaMemcpyHostToDevice);

	cudaMemcpy(d_doc_size,documents_size,number_documents*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_doc_label,docs_labels,number_documents*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_doc_ind,documents_indexes,number_documents*sizeof(int),cudaMemcpyHostToDevice);

	calculate_occurences_kernel<<<blocks,threads>>>(d_fv,d_occurences,d_doc_size,d_doc_ind,d_doc_label,pitch_occ,number_documents,number_unique_words,number_labels,d_weights);
	calculate_likelihood_kernel<<<blocks,threads>>>(d_fp,d_occurences,pitch_occ,pitch_fp,number_unique_words,number_labels);

	cudaMemcpy2D(h_fp,number_unique_words*sizeof(double),d_fp,pitch_fp,number_unique_words*sizeof(double),number_labels,cudaMemcpyDeviceToHost);

	first_parameter = CudaStd::convert_1d_to_2d_float(h_fp,number_labels,number_unique_words);

	free(h_fp);

	cudaFree(d_fv);
	cudaFree(d_doc_label);
	cudaFree(d_doc_size);
	cudaFree(d_doc_ind);
	cudaFree(d_fp);
	cudaFree(d_occurences);
	cudaFree(d_weights);
}

#else

void NaiveBayesClassifier::calculate_likelihood(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int number_unique_words, int number_documents, int number_labels, double* weights)
{
    printf("Calculating First parameter: \n");

	this->weights = weights;
	this->num_labels = number_labels;

	for(int i = 0;i < number_labels;i++)
	{
		int all_label_occ = calculate_all_words_label_occurence(feature_vectors, documents_size, documents_indexes, docs_labels,number_unique_words,number_documents,i);
		for(int j = 0; j < number_unique_words;j++)
		{
			first_parameter[i][j] = (calculate_single_word_label_occurence(feature_vectors, documents_size, documents_indexes, docs_labels ,j,number_documents,i) + 1)/
				(all_label_occ+number_unique_words);

			//if(j<30)
			//printf("%d ",calculate_single_word_label_occurence(feature_vectors, documents_size, documents_indexes, docs_labels ,j,number_documents,i));
		}
		//printf("\n");
	}
    
	//printf("\n");
}

double NaiveBayesClassifier::calculate_single_word_label_occurence(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int word_index, int number_documents, int label)
{
	double result = 0;

	for(int i = 0;i<number_documents;i++)
	{
		if(label == docs_labels[i])
		{
			for(int j = 0;j < documents_size[i];j++)
			{
				if(feature_vectors[documents_indexes[i] + j] == word_index)
				{
						result++;
				}
			}

		}
		else if(docs_labels[i] == -1 && (weights[i*num_labels + label] > EM_WEIGHTS_THRESHOLD) )
		{
			for(int j = 0;j < documents_size[i];j++)
			{
				if(feature_vectors[documents_indexes[i] + j] == word_index)
				{
						result += (weights[i*num_labels + label]*1);
				}
			}
		}
			
	}

	return result;

}

double NaiveBayesClassifier::calculate_all_words_label_occurence(int * feature_vectors, int * documents_size, int * documents_indexes, int * docs_labels, int number_unique_words, int number_documents, int label)
{
	double result = 0;

	for(int i = 0;i<number_unique_words;i++)
	{
		result += calculate_single_word_label_occurence(feature_vectors, documents_size, documents_indexes, docs_labels,i,number_documents,label);
	}

	return result;
}

#endif

#ifndef CUDA_CLASSIFY

int NaiveBayesClassifier::classify_unlabeled_document(int * unlabeled_feature_vector, int fv_size, int number_unique_words, int number_labels, double* prob_labels)
{
	double max_prob = N_INF;
	double min_prob = 1;
	int label = -1;
    
	for(int i = 0;i < number_labels;i++)
	{
		double prob = (double)NaiveBayesClassifier::prob_document_label(unlabeled_feature_vector, fv_size ,number_unique_words,i);

		if(prob_labels)
			prob_labels[i] = prob;

        if(prob > max_prob)
		{
			max_prob = prob;
			label = i;
		}

		if(prob < min_prob)
		{
			min_prob = prob;
		}
	}

	if(prob_labels)
	{
		for(int i = 0; i < number_labels;i++)
		{
			prob_labels[i] = (prob_labels[i] - min_prob)/(max_prob - min_prob);
		}
	}

	return label;
}

double NaiveBayesClassifier::prob_document_label(int * feature_vector, int fv_size, int number_unique_words, int label)
{
	double result = log10(second_parameter[label]);

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


__device__ double prob_document_label(int * feature_vector,double* fp, double*sp, int fv_size, int number_unique_words, int label)
{
	
	double result = log10((double)sp[label]);
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

__device__ int classify_unlabeled_document(int * unlabeled_feature_vector,double* fp, double* sp, int fv_size, int number_unique_words, int number_labels, double* prob_labels)
{
	double max_prob = N_INF;
	double min_prob = 1;
	int label = -1;
    
	for(int i = 0;i < number_labels;i++)
	{
		double prob = (double)prob_document_label(unlabeled_feature_vector, fp, sp, fv_size ,number_unique_words,i);

		if(prob_labels)
			prob_labels[i] = prob;

        if(prob > max_prob)
		{
			max_prob = prob;
			label = i;
		}

		if(prob < min_prob)
		{
			min_prob = prob;
		}
	}

	if(prob_labels)
	{
		for(int i = 0; i < number_labels;i++)
		{
			prob_labels[i] = (prob_labels[i] - min_prob)/(max_prob - min_prob);
		}
	}

	return label;
}

__global__ void classify_unlabeled_docs_kernel(int * unlabeled_feature_vectors, int* doc_sizes, int* docs_indexes, int num_docs ,double* fp, double* sp, int number_unique_words, int number_labels, int* docs_labels, double* prob_labels)
{
	int x = threadIdx.x + blockIdx.x*blockDim.x;

	if(x < num_docs)
	{
		if(prob_labels)
			docs_labels[x] = classify_unlabeled_document(unlabeled_feature_vectors+docs_indexes[x],fp,sp,doc_sizes[x],number_unique_words,number_labels,&prob_labels[x*number_labels]);
		else 
			docs_labels[x] = classify_unlabeled_document(unlabeled_feature_vectors+docs_indexes[x],fp,sp,doc_sizes[x],number_unique_words,number_labels,0);
	}
}




#endif

void NaiveBayesClassifier::classify_unlabeled_documents(int* feature_vectors, int* docs_sizes, int* docs_indexes, int num_documents, int num_unique_words, int num_labels, int* labels, double* prob_labels)
{
#ifndef CUDA_CLASSIFY
	for(int i = 0;i < num_documents;i++)
	{
		if(prob_labels)
			labels[i] = classify_unlabeled_document(feature_vectors+docs_indexes[i],docs_sizes[i],num_unique_words,num_labels,&prob_labels[i*num_labels]);
		else labels[i] = classify_unlabeled_document(feature_vectors+docs_indexes[i],docs_sizes[i],num_unique_words,num_labels,0);
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
	double* d_fp, *d_sp, *d_prob_labels;
	double *h_fp;

	cudaMalloc((void**)&d_fv,FV_SIZE*sizeof(int));
	cudaMalloc((void**)&d_docs_sizes,num_documents*sizeof(int));
	cudaMalloc((void**)&d_docs_indexes,num_documents*sizeof(int));
	cudaMalloc((void**)&d_labels,num_documents*sizeof(int));
	cudaMalloc((void**)&d_fp,num_unique_words*num_labels*sizeof(double));
	cudaMalloc((void**)&d_sp,num_labels*sizeof(double));
	cudaMalloc((void**)&d_prob_labels,num_documents*num_labels*sizeof(double));

	h_fp = CudaStd::convert_2d_to_1d_float(first_parameter,num_labels,num_unique_words);

	cudaMemcpy(d_fv,feature_vectors,FV_SIZE*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_docs_sizes,docs_sizes,num_documents*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_docs_indexes,docs_indexes,num_documents*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_fp,h_fp,num_unique_words*num_labels*sizeof(double),cudaMemcpyHostToDevice);
	cudaMemcpy(d_sp,second_parameter,num_labels*sizeof(double),cudaMemcpyHostToDevice);

	classify_unlabeled_docs_kernel<<<blocks,threads>>>(d_fv,d_docs_sizes,d_docs_indexes,num_documents,d_fp,d_sp,num_unique_words,num_labels,d_labels,d_prob_labels);

	cudaMemcpy(labels,d_labels,num_documents*sizeof(int),cudaMemcpyDeviceToHost);
	cudaMemcpy(prob_labels,d_prob_labels,num_documents*num_labels*sizeof(double),cudaMemcpyDeviceToHost);

	free(h_fp);

	cudaFree(d_fv);
	cudaFree(d_docs_sizes);
	cudaFree(d_docs_indexes);
	cudaFree(d_labels);
	cudaFree(d_fp);
	cudaFree(d_sp);
	cudaFree(d_prob_labels);

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
		second_parameter[i] = (double)(1+calculate_label_occurance(documents_labels,number_documents,i))/
			(double)denom;
        //printf("%lf \n",second_parameter[i]);
	}
    
    //printf("\n");

}

double ** NaiveBayesClassifier::get_likelihood()
{
    return first_parameter;
}

double *  NaiveBayesClassifier::get_prior()
{
    return second_parameter;
}