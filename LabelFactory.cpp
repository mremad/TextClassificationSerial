//
//  LabelFactory.cpp
//  TextClassificationSerial
//
//  Created by Mohamed Emad on 6/11/14.
//  Copyright (c) 2014 Mohamed Emad. All rights reserved.
//

#include "LabelFactory.h"

LabelFactory::LabelFactory(int** feature_vectors,int total_number_documents,int desired_labeled_documents,int num_labels)
{
	labeled_fv = (int**)malloc(sizeof(int*)*desired_labeled_documents);
    unlabeled_fv = (int**)malloc(sizeof(int*)*(total_number_documents - desired_labeled_documents));
	this->feature_vectors=feature_vectors;
	this->total_number_documents=total_number_documents;
	this->desired_labeled_documents=desired_labeled_documents;
	this->num_labels=num_labels; 
	delete_documents_labels();
}

void LabelFactory::delete_documents_labels()
{
    create_labeled_fv();
}

void LabelFactory::create_unlabeled_fv()
{
    
}

void LabelFactory::create_labeled_fv()
{
	int index_in_fv;
    for(int i=0;i<desired_labeled_documents;i++)
	{
		do{
			srand(time( NULL ));
			index_in_fv=(rand() % total_number_documents);
		}while(feature_vectors[index_in_fv][0]==-1);
		labeled_fv[i]=feature_vectors[index_in_fv];
		feature_vectors[index_in_fv][0]=-1;
	}
	for(int j=0;j<desired_labeled_documents;j++)
		for(int k=0;k<3;k++)
		{
			printf("[%i][%i]=%i\n",j,k,labeled_fv[j][k]);
		}
}

void LabelFactory::check_labeled_count()
{
    
}