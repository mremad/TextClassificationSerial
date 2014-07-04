//
//  LabelFactory.h
//  TextClassificationSerial
//
//  Created by Mohamed Emad on 6/11/14.
//  Copyright (c) 2014 Mohamed Emad. All rights reserved.
//
#ifndef _LABELFACTORY
#define _LABELFACTORY

#include "stdlib.h"
#include <time.h>
#include <stdio.h>
class LabelFactory
{
	void create_unlabeled_fv();
	void create_labeled_fv();
	void check_labeled_count();
	void initialize(int* documents_labels,int total_number_documents,int desired_labeled_documents,int num_labels);
	int num_labels;
	int desired_labeled_documents;
	int total_number_documents;
	int* processed_vectors;
	int* label_frequency;
	int* label_index_in_FV;
	int* documents_labels;
    public:
	LabelFactory();
    void select_labeled_docs(int* documents_labels,int total_number_documents,int desired_labeled_documents,int num_labels);
    int* labeled_fv;
    int* unlabeled_fv;
};

#endif