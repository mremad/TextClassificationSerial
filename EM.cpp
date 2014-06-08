#include "EM.h"


void EM::run_em(NaiveBayesClassifier* classifier, int** labeled_feature_vectors, int** unlabeled_feature_vectors, int number_unique_words, int number_documents, int number_labels)
{
	/*Initial Step*/
	//Construct classifier with labeled feature_vectors

	while(true)
	{
		/*E Step*/
		//classifier->classify();

		/*M Step*/
		//Handles merging of labeled/unlabeled feature_vectors

		//classifier->calculate_first_parameter();

		//classifier->calculate_second_parameter();

		/*Check for convergence*/
		//break;
		
	}
}