//
//  LabelFactory.cpp
//  TextClassificationSerial
//
//  Created by Mohamed Emad on 6/11/14.
//  Copyright (c) 2014 Mohamed Emad. All rights reserved.
//

#include "LabelFactory.h"

LabelFactory::LabelFactory()
{
    
}

void LabelFactory::delete_documents_labels(int** feature_vectors, int total_number_documents)
{
    
}

void LabelFactory::select_labeled_docs(int** feature_vectors, int total_number_documents, int desired_labeled_documents, int number_labels)
{
    labeled_fv = (int**)malloc(sizeof(int*)*desired_labeled_documents);
    unlabeled_fv = (int**)malloc(sizeof(int*)*(total_number_documents - desired_labeled_documents));
    
    
    /* Logic to pick labeled documents and unlabeled documents*/
    
    
    //Delete the labels of other documents
    delete_documents_labels(feature_vectors, total_number_documents);
    
}