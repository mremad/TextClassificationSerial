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
	processed_vectors=(int*)malloc(sizeof(int)*total_number_documents);
	label_frequency=(int*)malloc(sizeof(int)*desired_labeled_documents);
	label_index_in_FV=(int*)malloc(sizeof(int)*desired_labeled_documents);
	for(int i=0;i<desired_labeled_documents;i++)
	{
		label_frequency[i]=0;
	}
	for(int i=0;i<total_number_documents;i++)
	{
		processed_vectors[i]=0;
	}
	this->feature_vectors=feature_vectors;
	this->total_number_documents=total_number_documents;
	this->desired_labeled_documents=desired_labeled_documents;
	this->num_labels=num_labels; 
	for(int i=0;i<total_number_documents;i++)
	{
		printf("\nTotal FV={%i,%i,%i}",feature_vectors[i][0],feature_vectors[i][1],feature_vectors[i][2]);
	}
	for(int i=0;i<desired_labeled_documents;i++)
	{
		printf("\nLabeled FV={%i,%i,%i}",labeled_fv[i][0],labeled_fv[i][1],labeled_fv[i][2]);
	}
	for(int i=0;i<total_number_documents-desired_labeled_documents;i++)
	{
		printf("\nunLabeled FV={%i,%i,%i}",unlabeled_fv[i][0],unlabeled_fv[i][1],unlabeled_fv[i][2]);
	}
}

void LabelFactory::delete_documents_labels()
{
    create_labeled_fv();
	check_labeled_count();
	create_unlabeled_fv();
}

void LabelFactory::create_unlabeled_fv()
{
	int index_in_unlabeled_fv=0;
    for(int i=0;i<total_number_documents;i++)
	{
		if(processed_vectors[i]==0)
		{
			unlabeled_fv[index_in_unlabeled_fv]=feature_vectors[i];
			processed_vectors[i]=1;
			index_in_unlabeled_fv++;
		}
	}
}

void LabelFactory::create_labeled_fv()
{
	int index_in_fv;
    for(int i=0;i<desired_labeled_documents;i++)
	{
		do{
			srand(time( NULL ));
			index_in_fv=(rand() % total_number_documents);
		}while(processed_vectors[index_in_fv]==1);
		labeled_fv[i]=feature_vectors[index_in_fv];
		processed_vectors[index_in_fv]=1;
		label_index_in_FV[i]=index_in_fv;
		label_frequency[labeled_fv[i][0]]++;
	}
	for(int i=0;i<desired_labeled_documents;i++)
	{
		printf("\nBefore count check Labeled FV={%i,%i,%i}",labeled_fv[i][0],labeled_fv[i][1],labeled_fv[i][2]);
	}
}

void LabelFactory::check_labeled_count()
{
    for(int i=0;i<num_labels;i++)
		for(int j=0;j<desired_labeled_documents;j++)
			{
				if(labeled_fv[j][0]==i)
				{
					break;
				}
				if(j==desired_labeled_documents-1)
				{
					bool done=false;
					printf("\nMissing label %i \n",i);
					for(int k=0;k<total_number_documents;k++)
					{
						if(feature_vectors[k][0]==i && processed_vectors[k]==0)
						{
							processed_vectors[k]=1;
							for(int x=0;x<desired_labeled_documents;x++)
							{
								if(label_frequency[x]>1)
								{
									for(int y=0;y<desired_labeled_documents;y++)
									{
										if(labeled_fv[y][0]==x)
										{
											labeled_fv[y]=feature_vectors[k];
											processed_vectors[label_index_in_FV[y]]=0;
											label_index_in_FV[y]=k;
											done=true;
											break;
										}
									}
								}
								if(done)
									break;
							}
						}
						if (done)
							break;
					}
				}
			}
	
}