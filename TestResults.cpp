#include "TestResults.h"

TestResults::TestResults(string document_path,int number_documents,FeatureConstructor* feature_constructor, NaiveBayesClassifier* classifier)
{
	this->document_path=document_path;
	this->number_documents=number_documents;
	this->feature_constructor=feature_constructor;
	this->classifier=classifier;
	result=(int*)malloc(sizeof(int)*number_documents);
	accuracy=0;
}

void TestResults::start_test()
{
	read_documents();
	extract_feature_vector();
	classify_documents();
    check_accuracy();
	printf("Accuracy = %f%%\n",accuracy);
}

void TestResults::read_documents()
{
	FileReader fr = FileReader(number_documents,document_path);
	fr.read_files();
	data_list=fr.data_list;
	documents_size=fr.documents_size;
}

void TestResults::extract_feature_vector()
{
	printf("FROM TEST RESULTS BEFORE: %d , %d\n", feature_constructor->total_char_count,feature_constructor->total_word_count);
	feature_constructor->process_data_list(data_list,documents_size,number_documents);
	printf("FROM TEST RESULTS AFTER: %d , %d\n", feature_constructor->total_char_count,feature_constructor->total_word_count);
	//feature_constructor->construct_feature_vectors(data_list,documents_size,number_documents,feature_constructor->total_char_count,feature_constructor->total_word_count);
	feature_constructor->construct_feature_vectors(data_list,documents_size,number_documents);
	feature_vector=feature_constructor->feature_vector;
}

void TestResults::classify_documents()
{
    printf("Classifying Test Data...\n");

	classifier->classify_unlabeled_documents(feature_vector,documents_size,feature_constructor->documents_indexes,number_documents,feature_constructor->num_unique_words,feature_constructor->num_labels,result,0);

    printf("Done Classifying Test Data\n");
    //printf("\n");
}

void TestResults::check_accuracy()
{
	for(int i=0;i<number_documents;i++)
	{
		if(result[i]==feature_constructor->documents_labels[i])
		{
			accuracy++;
		}

		//printf("%d  %d  \n", result[i],feature_constructor->documents_labels[i]);
	}
	accuracy=(accuracy/(float)number_documents)*100;
}
