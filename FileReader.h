#ifndef _FILEREADER
#define _FILEREADER

#include "ConsolePrint.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
class FileReader
{
	int num_docs;
	int processed_docs;
	string path;
	void read_single_file(string line, string* document_words);
	public:
		FileReader(int docs_to_read,string path);
		//2D array of all the words in all the documents. Size: MxD
		string** data_list;
		
		//1D array of the size of each document
		int* documents_size_labeled;
		int* documents_size_unlabeled;
		int* documents_size;

		void read_files();
        void read_files_per_label(int docs_per_label, int num_labels);
		void label_factory(int desired_labeled, int desired_unlabeled, int num_labels, string** Udata_out, string** Ldata_out);
		void deallocate();
		
};

#endif