#include "EM.h"
#include "Cfg.h"

EM::EM()
{

}

void EM::classify_all_unlabeled_documents(NaiveBayesClassifier* classifier, int* feature_vectors,int* docs_sizes, int* docs_ind, int number_unique_words, int number_documents, int number_labels, int* result)
{
	classifier->classify_unlabeled_documents(feature_vectors,docs_sizes,docs_ind,number_documents,number_unique_words,number_labels,result,weights_unlabeled);

	for(int i = 0;i<number_documents;i++)
	{
		all_doc_labels[unlabeled_docs[i]] = -1;
	}
}

void EM::copy_parameters(double** src_likelihood, double* src_prior, double** dest_likelihood, double* dest_prior, int number_labels, int number_unique_words)
{
    for(int i = 0;i<number_labels;i++)
    {
        for(int j = 0;j<number_unique_words;j++)
        {
            dest_likelihood[i][j] = src_likelihood[i][j];
        }
        
        dest_prior[i] = src_prior[i];
    }
}

void EM::copy_labels(int* new_labels, int* old_labels, int num_labels)
{

}

bool EM::check_if_converged(double** old_likelihood, double* old_prior, double** new_likelihood, double* new_prior, int number_labels, int number_unique_words)
{
    bool result = false;
    
    double total_diff = 0;
    
    double threshold = EM_CONVERGENCE_THRESHOLD;
    
    for(int i = 0;i<number_labels;i++)
    {
        for(int j = 0;j<number_unique_words;j++)
        {
            total_diff += fabs(old_likelihood[i][j]-new_likelihood[i][j]);
        }
        
        total_diff += fabs(old_prior[i]-new_prior[i]);
    }
    
    printf("Parameter Iteration Difference: %f\n",total_diff);
    
    if(total_diff < threshold)
        result = true;
    
    return result;
}

int EM::calculate_size_feature_vector(int* docs_ind,int* docs_sizes, int num_docs)
{
	int size = 0;

	for(int i = 0;i < num_docs;i++)
	{
		size += docs_sizes[docs_ind[i]];
	}

	return size;
}

void EM::build_seperate_fv(int* fv, int* docs_sizes, int* docs_ind, int* docs_labels, int* labeled_docs, int* unlabeled_docs, int num_labeled, int num_unlabeled)
{
	int curr_ind = 0;
	for(int i = 0; i < num_labeled;i++)
	{
		for(int j = 0;j < docs_sizes[labeled_docs[i]];j++)
		{
			labeled_fv[curr_ind + j] = fv[docs_ind[labeled_docs[i]] + j];
		}

		labeled_doc_labels[i] = docs_labels[labeled_docs[i]];
		labeled_docs_sizes[i] = docs_sizes[labeled_docs[i]];
		labeled_docs_ind[i] = curr_ind;

		curr_ind += docs_sizes[labeled_docs[i]];
	}

	curr_ind = 0;

	for(int i = 0;i < num_unlabeled;i++)
	{
		for(int j = 0;j < docs_sizes[unlabeled_docs[i]];j++)
		{
			unlabeled_fv[curr_ind + j] = fv[docs_ind[unlabeled_docs[i]] + j];
		}

		unlabeled_doc_labels[i] = docs_labels[unlabeled_docs[i]];
		unlabeled_docs_sizes[i] = docs_sizes[unlabeled_docs[i]];
		unlabeled_docs_ind[i] = curr_ind;

		curr_ind += docs_sizes[unlabeled_docs[i]];
	}
}

void EM::initialize_weights(int num_labeled_docs, int num_unlabeled_docs, int num_labels)
{
	weights = (double*)malloc(sizeof(double)*(num_labeled_docs + num_unlabeled_docs)*num_labels);
	weights_unlabeled = (double*)malloc(sizeof(double)*num_unlabeled_docs*num_labels);

	for(int i = 0; i < num_labeled_docs;i++)
	{
		weights[labeled_docs[i]] = -1;
	}

	for(int i = 0;i < num_unlabeled_docs;i++)
	{
		weights[unlabeled_docs[i]] = -1;
		weights_unlabeled[i] = -1;
	}

}

void EM::update_total_weights(int num_unlabeled_docs, int num_labels)
{
	for(int i = 0;i < num_unlabeled_docs;i++)
	{
		for(int j = 0; j < num_labels;j++)
			weights[unlabeled_docs[i]*num_labels + j] = weights_unlabeled[i*num_labels + j];
	}
}

void EM::estimate_weights(int* old_labels, int* new_labels, int num_docs)
{
	for(int i = 0;i < num_docs;i++)
	{
		if(old_labels[i] == new_labels[i])
		{
			weights[unlabeled_docs[i]]++;
		}
		else
		{
			weights[unlabeled_docs[i]]--;

			if(weights[unlabeled_docs[i]] < 0)
				weights[unlabeled_docs[i]] = 0;
		}
	}
}

void EM::run_em(NaiveBayesClassifier* classifier, int* feature_vectors,int* docs_sizes, int* docs_ind,int* docs_labels, int* labeled_docs, int* unlabeled_docs, int number_unique_words,int number_unlabeled_documents, int number_labeled_documents, int number_labels)
{
    
    /*Initialize arrays to store old parameters*/
    
    double**	old_likelihood	= (double**)malloc(sizeof(double*)*number_labels);
    double*	old_prior		= (double*)malloc(sizeof(double)*number_labels);
	int*			old_labels		= (int*)malloc(sizeof(int)*number_unlabeled_documents);

	labeled_fv = (int*)malloc(sizeof(int)*calculate_size_feature_vector(labeled_docs,docs_sizes,number_labeled_documents));
	unlabeled_fv = (int*)malloc(sizeof(int)*calculate_size_feature_vector(unlabeled_docs,docs_sizes,number_unlabeled_documents));

	labeled_docs_sizes = (int*)malloc(sizeof(int)*number_labeled_documents);
	unlabeled_docs_sizes = (int*)malloc(sizeof(int)*number_unlabeled_documents);

	labeled_docs_ind = (int*)malloc(sizeof(int)*number_labeled_documents);
	unlabeled_docs_ind = (int*)malloc(sizeof(int)*number_unlabeled_documents);

	labeled_doc_labels = (int*)malloc(sizeof(int)*number_labeled_documents);
	unlabeled_doc_labels = (int*)malloc(sizeof(int)*number_unlabeled_documents);
    
    for(int i = 0;i<number_labels;i++)
        old_likelihood[i] = (double*)malloc(sizeof(double)*number_unique_words);
    
	all_doc_labels = docs_labels;
	this->labeled_docs = labeled_docs;
	this->unlabeled_docs = unlabeled_docs;

	build_seperate_fv(feature_vectors,docs_sizes,docs_ind,docs_labels,labeled_docs,unlabeled_docs,number_labeled_documents,number_unlabeled_documents);
	
	initialize_weights(number_labeled_documents, number_unlabeled_documents,number_labels);
	
	/*Initial Step*/
	//Construct classifier with labeled feature_vectors
	classifier->calculate_likelihood(labeled_fv,labeled_docs_sizes,labeled_docs_ind,labeled_doc_labels,number_unique_words,number_labeled_documents,number_labels, 0);

	classifier->calculate_prior(labeled_doc_labels,number_labeled_documents,number_labels);
    
	for(int i = 0; i < EM_MAX_ITERATIONS;i++)
	{
		
		printf("Iteration number: %d\n",(i+1));

		/*E Step*/
        printf("\t\tPerforming E Step\n");

		classify_all_unlabeled_documents(classifier,unlabeled_fv,unlabeled_docs_sizes,unlabeled_docs_ind,number_unique_words,number_unlabeled_documents,number_labels,unlabeled_doc_labels);
        
        //ConsolePrint::print_2d_int(number_unique_words, number_unlabeled_documents, unlabeled_docs);
        //ConsolePrint::print_2d_int(number_unique_words, number_labeled_documents, labeled_docs);
        //ConsolePrint::print_2d_int(number_unique_words, number_labeled_documents+number_unlabeled_documents, feature_vectors);
		
		update_total_weights(number_unlabeled_documents,number_labels);

        /*M Step*/
        
        printf("\t\tPerforming M Step\n");

        copy_parameters(classifier->get_likelihood(), classifier->get_prior(), old_likelihood, old_prior, number_labels, number_unique_words);
		copy_labels(unlabeled_doc_labels, old_labels, number_unlabeled_documents);

		classifier->calculate_likelihood(feature_vectors,docs_sizes,docs_ind,docs_labels,number_unique_words,number_labeled_documents+number_unlabeled_documents,number_labels, weights);

		classifier->calculate_prior(docs_labels,number_labeled_documents+number_unlabeled_documents,number_labels);

		/*Check for convergence*/
        
        if(check_if_converged(old_likelihood,old_prior,classifier->get_likelihood(),classifier->get_prior(),number_labels,number_unique_words))
            break;
        
		
	}

    for(int i = 0;i<number_labels;i++)
		free(old_likelihood[i]);

	free(old_likelihood);
	free(old_prior);
	free(old_labels);

    printf("EM Process Done\n");
}

void EM::deallocate()
{
	free( labeled_fv );
	free( unlabeled_fv);

	free( labeled_docs_sizes);
	free( unlabeled_docs_sizes);

	free( labeled_docs_ind);
	free( unlabeled_docs_ind);

	free( labeled_doc_labels);
	free( unlabeled_doc_labels);
	free( all_doc_labels);

	free( labeled_docs);
	free( unlabeled_docs);

	if(weights)
		free( weights);

	if(weights_unlabeled)
		free( weights_unlabeled);
}
