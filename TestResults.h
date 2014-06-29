#ifndef _TESTRESULTS
#define _TESTRESULTS

#include "FileReader.h"
#include "FeatureConstructor.h"
#include "EM.h"
class TestResults
{
	FeatureConstructor* feature_constructor;
	NaiveBayesClassifier* classifier;
	int* documents_size;
	int* feature_vector;
	int* result;
	float accuracy;
	string document_path;
	string** data_list;
	int number_documents;
	void read_documents();
	void extract_feature_vector();
	void classify_documents();
	void check_accuracy();
public:
	TestResults(string document_path,int number_documents,FeatureConstructor* feature_constructor,NaiveBayesClassifier* classifier);
	void start_test();
};

#endif