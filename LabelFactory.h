//
//  LabelFactory.h
//  TextClassificationSerial
//
//  Created by Mohamed Emad on 6/11/14.
//  Copyright (c) 2014 Mohamed Emad. All rights reserved.
//

#include "stdlib.h"
#include <time.h>
#include <stdio.h>
class LabelFactory
{
    void delete_documents_labels();
	void create_unlabeled_fv();
	void create_labeled_fv();
	void check_labeled_count();
	int num_labels;
	int desired_labeled_documents;
	int total_number_documents;
	int** feature_vectors;
    public:
    LabelFactory(int** feature_vectors,int total_number_documents,int desired_labeled_documents,int num_labels);
    int** labeled_fv;
    int** unlabeled_fv;
};