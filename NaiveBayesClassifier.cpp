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

void NaiveBayesClassifier::calculate_likelihood(int** feature_vectors,int number_unique_words, int number_documents, int number_labels)
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

