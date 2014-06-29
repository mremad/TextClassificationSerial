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
	feature_constructor->construct_feature_vectors(data_list,documents_size,number_documents);
	feature_vector=feature_constructor->feature_vector;
}

void TestResults::classify_documents()
{
    printf("Classifying Test Data...\n");
	for(int i=0;i<number_documents;i++)
	{
		result[i]=classifier->classify_unlabeled_document(feature_vector,feature_constructor->num_unique_words,feature_constructor->num_labels);
        //printf("%d ",result[i]);
	}
    printf("Done Classifying Test Data\n");
    //printf("\n");
}

void TestResults::check_accuracy()
{
	for(int i=0;i<number_documents;i++)
	{
		if(result[i]==feature_vector[i])
		{
			accuracy++;
		}
	}
	accuracy=(accuracy/(float)number_documents)*100;
}
