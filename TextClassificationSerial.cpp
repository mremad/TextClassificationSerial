// TextClassificationSerial.cpp : Defines the entry point for the console application.
//
 
#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"
#include "LabelFactory.h"
#include "TestResults.h"
#include "Cfg.h"
 
 
int main(int argc, char* argv[])
{
    int start_time;
        int end_time;
        int document_size = 11290; // max 11290
    int desired_labeled = 100; // max 5500
    int num_labels = 20;
   
    int test_documents = 7500;
               
        FileReader fr = FileReader(document_size,DATA_PATH_SHAABAN);
    fr.read_files();
    fr.read_files_per_label(desired_labeled/num_labels, num_labels);
   
        document_size = desired_labeled;
        desired_labeled = 20;
 
        FeatureConstructor fc = FeatureConstructor(fr.documents_size,document_size,fr.total_word_count,fr.total_char_count);
        fc.process_data_list(fr.data_list,fr.documents_size,document_size);
    //fc.extract_vocab(fr.data_list, fr.documents_size, document_size);
        fc.extract_vocab(fr.data_list,fr.documents_size,document_size,fc.total_word_count,fc.total_char_count);
    //fc.construct_feature_vectors(fr.data_list, fr.documents_size, document_size);
        fc.construct_feature_vectors(fr.data_list,fr.documents_size, document_size,fc.total_char_count, fc.total_word_count);
 
        fr.deallocate();
    LabelFactory lf = LabelFactory();
        lf.select_labeled_docs(fc.documents_labels,document_size,desired_labeled,fc.num_labels);
   
        NaiveBayesClassifier nc = NaiveBayesClassifier(fc.num_labels,fc.num_unique_words);
   
        //start_time = clock();
        //nc.calculate_likelihood(fc.feature_vector, fc.documents_size, fc.documents_indexes, fc.documents_labels, fc.num_unique_words, document_size, fc.num_labels);
        //end_time = clock();
        //printf("Time to calculate: %f\n",((end_time - start_time)/(float)CLOCKS_PER_SEC));
        //nc.calculate_prior(fc.documents_labels, desired_labeled, fc.num_labels);
   
        //ConsolePrint::print_2d_float(fc.num_unique_words, 1, nc.get_likelihood());
   
        start_time = clock();
    EM em = EM();
        em.run_em(&nc,fc.feature_vector,fc.documents_size,fc.documents_indexes,fc.documents_labels,lf.labeled_fv,lf.unlabeled_fv,fc.num_unique_words,document_size-desired_labeled,desired_labeled,fc.num_labels);
    end_time = clock();
        printf("Time to calculate: %f\n",((end_time - start_time)/(float)CLOCKS_PER_SEC));
 
    TestResults tr = TestResults(TEST_PATH_SHAABAN, test_documents, &fc, &nc);
    tr.start_test();
 
    printf("Program terminated safely\n");
        getchar();
        return 0;
}