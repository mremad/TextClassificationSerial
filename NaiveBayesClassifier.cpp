#include "NaiveBayesClassifier.h"

NaiveBayesClassifier::NaiveBayesClassifier(int number_labels,int number_unique_words)
{
	first_parameter = (float**)malloc(sizeof(float*)*number_labels);
	second_parameter = (float*)malloc(sizeof(float)*number_labels);

	for(int i = 0;i<number_labels;i++)
		first_parameter[i] = (float*)malloc(sizeof(float)*number_unique_words);


}

void NaiveBayesClassifier::calculate_first_parameter(int** feature_vectors,int number_unique_words, int number_documents, int number_labels)
{
    printf("Calculating First parameter: \n");
	for(int i = 0;i < number_labels;i++)
	{
		int all_label_occ = calculate_all_words_label_occurence(feature_vectors,number_unique_words,number_documents,i);
		for(int j = 1; j < number_unique_words;j++)
		{
			first_parameter[i][j-1] = (float)(calculate_single_word_label_occurence(feature_vectors,j,number_documents,i) + 1) *1 /
				(float)(all_label_occ+number_unique_words);

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
			result += feature_vectors[i][word_index];
	}

	return result;

}

int NaiveBayesClassifier::calculate_all_words_label_occurence(int ** feature_vectors, int number_unique_words, int number_documents, int label)
{
	int result = 0;

	for(int i = 1;i<number_unique_words;i++)
	{
		result += calculate_single_word_label_occurence(feature_vectors,i,number_documents,label);
	}

	return result;
}

void NaiveBayesClassifier::calculate_second_parameter(int** feature_vectors, int number_documents, int number_labels)
{
    printf("Calculating Second Parameter: \n");
	int denom = number_documents+number_labels;
    
	for(int i = 0;i<number_labels;i++)
	{
		second_parameter[i] = (float)(1+calculate_label_occurance(feature_vectors,number_documents,i))/
			(float)denom;
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
	long double max_prob = 0;
	int label = -1;

	for(int i = 0;i < number_labels;i++)
	{
		long double prob = (long double)NaiveBayesClassifier::prob_document_label(unlabeled_feature_vector,number_unique_words,i)/
			(long double)prob_document_all_labels(unlabeled_feature_vector,number_unique_words,number_labels);

		if(prob > max_prob)
		{
			max_prob = prob;
			label = i;
		}
	}

	unlabeled_feature_vector[0] = label;
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

long double power(float base, int exp)
{
	long double result = 1;
	for(int i = 0;i<exp;i++)
	{
		result = result * base;
	}

	return result;
}

long double NaiveBayesClassifier::prob_document_label(int * feature_vector, int number_unique_words, int label)
{
	long double result = second_parameter[label];

	//printf("result: %f \n",result);

	for(int i = 1;i<number_unique_words;i++)
	{
		if(first_parameter[label][i] == 0)
			continue;
		result = result * power(first_parameter[label][i],feature_vector[i]);
		//printf("result:  %f %d = %e \n",first_parameter[label][i], feature_vector[i] ,result);
	}

	return result;
}

