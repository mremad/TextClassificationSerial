//
//  LabelFactory.h
//  TextClassificationSerial
//
//  Created by Mohamed Emad on 6/11/14.
//  Copyright (c) 2014 Mohamed Emad. All rights reserved.
//

#include "stdlib.h"
class LabelFactory
{
    void delete_documents_labels(int** feature_vectors, int total_number_documents);
    public:
    LabelFactory();
    int* labeled_docs_indexes;
    void select_labeled_docs(int** feature_vectors, int total_number_documents, int desired_labeled_documents, int number_labels);
};