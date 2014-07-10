// TextClassificationSerial.cpp : Defines the entry point for the console application.
//
 
#include <stdio.h>
#include "EM.h"
#include "FeatureConstructor.h"
#include "FileReader.h"
#include "LabelFactory.h"
#include "TestResults.h"
#include "Cfg.h"
#include "CudaStd.h"


int main(int argc, char* argv[])
{
	FILE *stream ;
   if((stream = freopen("Log.txt", "w", stdout)) == NULL)
      exit(-1);

    int start_time;
    int end_time;
	int time_serial;
	int time_parallel;
	
	int total_time=0;

    int desired_documents; // max 5500
	int desired_labeled;
    int num_labels = 20;
   
    int test_documents = 7500;
	
	for(int i=0;i<7;i++)
	{
       
		switch(i)
		{
		case 0:
			desired_documents=40;
			desired_labeled=20;
			stream = freopen("/dev/tty", "a", stdout);
			printf("---------------\n");
			printf("Test Case #%d - %d Total Documents - %d Labeled Documents", 1 ,desired_documents, desired_labeled);
			printf("Started running...\n");

			stream = freopen("Log.txt", "a", stdout);
			printf("---------------\n");
			printf("Test Case #%d - %d Total Documents - %d Labeled Documents", 1 ,desired_documents, desired_labeled);
			printf("*****Running serial version*****\n");

			CudaStd::cuda_ver = CODE_SERIAL;
			break;
		case 1:
			time_serial=total_time;
			desired_documents=40;
			desired_labeled=20;
			stream = freopen("Log.txt", "a", stdout);
			printf("*****Running CUDA version*****\n");
			CudaStd::cuda_ver = CODE_CUDA;
			break;
		case 2:

			stream = freopen("/dev/tty", "a", stdout);
			printf("Speedup: %f \n",(time_serial)/(float)total_time);

			stream = freopen("Log.txt", "a", stdout);
			printf("Speedup: %f \n",(time_serial)/(float)total_time);

			desired_documents=60;
			desired_labeled=40;

			stream = freopen("/dev/tty", "a", stdout);
			printf("---------------\n");
			printf("Test Case #%d - %d Total Documents - %d Labeled Documents", 2 ,desired_documents, desired_labeled);
			printf("Started running...\n");

			stream = freopen("Log.txt", "a", stdout);
			printf("---------------\n");
			printf("Test Case #%d - %d Total Documents - %d Labeled Documents", 2 ,desired_documents, desired_labeled);
			printf("*****Running serial version*****\n");

			CudaStd::cuda_ver = CODE_SERIAL;
			break;
		case 3:
			time_serial=total_time;
			desired_documents=60;
			desired_labeled=40;
			
			stream = freopen("Log.txt", "a", stdout);
			printf("Running CUDA version: \n");

			CudaStd::cuda_ver = CODE_CUDA;
			break;
		case 4:
			
			stream = freopen("/dev/tty", "a", stdout);
			printf("Speedup: %f \n",(time_serial)/(float)total_time);

			stream = freopen("Log.txt", "a", stdout);
			printf("Speedup: %f \n",(time_serial)/(float)total_time);

			desired_documents=80;
			desired_labeled=60;
			
			stream = freopen("/dev/tty", "a", stdout);
			printf("---------------\n");
			printf("Test Case #%d - %d Total Documents - %d Labeled Documents", 3 ,desired_documents, desired_labeled);
			printf("Started running...\n");

			stream = freopen("Log.txt", "a", stdout);
			printf("---------------\n");
			printf("Test Case #%d - %d Total Documents - %d Labeled Documents", 3 ,desired_documents, desired_labeled);
			printf("*****Running serial version*****\n");


			CudaStd::cuda_ver = CODE_SERIAL;
			break;
		case 5:
			time_serial=total_time;
			desired_documents=80;
			desired_labeled=60;

			stream = freopen("Log.txt", "a", stdout);
			printf("Running CUDA version: \n");

			CudaStd::cuda_ver = CODE_CUDA;
			break;
		case 6:
			
			stream = freopen("/dev/tty", "a", stdout);
			printf("Speedup: %f \n",(time_serial)/(float)total_time);

			stream = freopen("Log.txt", "a", stdout);
			printf("Speedup: %f \n",(time_serial)/(float)total_time);

			desired_documents=5500;
			desired_labeled=500;

			printf("---------------\n");
			printf("Test Case #%d - %d Total Documents - %d Labeled Documents", 4 ,desired_documents, desired_labeled);
			printf("Started running CUDA only...\n");

			stream = freopen("Log.txt", "a", stdout);
			printf("---------------\n");
			printf("Test Case #%d - %d Total Documents - %d Labeled Documents", 4 ,desired_documents, desired_labeled);
			printf("*****Running CUDA version*****\n");

			CudaStd::cuda_ver = CODE_CUDA;
			break;
		}
		total_time=0;
		start_time = clock();

		int document_size = 11290;// max 11290
		FileReader fr = FileReader(document_size,DATA_PATH_SHAABAN);
		fr.read_files();
		fr.read_files_per_label(desired_documents/num_labels, num_labels);

   
		document_size = desired_documents;
	
		FeatureConstructor fc = FeatureConstructor(fr.documents_size,document_size);

		fc.process_data_list(fr.data_list,fr.documents_size,document_size);

		end_time = clock();

		total_time+=(end_time - start_time);
    
		
		start_time = clock();

		fc.extract_vocab(fr.data_list,fr.documents_size,document_size);

		end_time = clock();
		total_time+=(end_time - start_time);
		printf("Time to Extract Vocab List: %f\n",((end_time - start_time)/(float)CLOCKS_PER_SEC));
	

		start_time = clock();

		fc.construct_feature_vectors(fr.data_list,fr.documents_size, document_size);

		end_time = clock();
		total_time+=(end_time - start_time);
		printf("Time to Extract Feature Vectors: %f\n",((end_time - start_time)/(float)CLOCKS_PER_SEC));

		start_time = clock();
		fr.deallocate();
		
		LabelFactory lf = LabelFactory();
		lf.select_labeled_docs(fc.documents_labels,document_size,desired_labeled,fc.num_labels);

		NaiveBayesClassifier nc = NaiveBayesClassifier(fc.num_labels,fc.num_unique_words);
		end_time = clock();
		total_time+=(end_time - start_time);

		start_time = clock();
		EM em = EM();
		em.run_em(&nc,fc.feature_vector,fc.documents_size,fc.documents_indexes,fc.documents_labels,lf.labeled_fv,lf.unlabeled_fv,fc.num_unique_words,document_size-desired_labeled,desired_labeled,fc.num_labels);
		end_time = clock();
		printf("Time to Calculate: %f\n",((end_time - start_time)/(float)CLOCKS_PER_SEC));
		total_time+=(end_time - start_time);

		printf("Starting Test Results\n");
		start_time = clock();
		TestResults tr = TestResults(TEST_PATH_SHAABAN, test_documents, &fc, &nc);
		tr.start_test();
		total_time+=(end_time - start_time);
		start_time = clock();

	
		printf("Time to Code : %f\n",total_time/(float)CLOCKS_PER_SEC);

		//nc.deallocate();
		//em.deallocate();

		//delete &nc;
		//delete &em;
		//delete &fc;
		//delete &fr;

	}



	stream = freopen("/dev/tty", "a", stdout);
    printf("Program terminated safely\n");
	stream = freopen("Log.txt", "a", stdout);
    printf("Program terminated safely\n");
    getchar();
    return 0;
}

