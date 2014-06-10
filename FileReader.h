#include "ConsolePrint.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class FileReader
{
	int labeled;
	int unlabeled;
	int processedDocs;
	void read_single_file(string line, string* document_words);
	public:
		FileReader(int labeled,int unlabeled);
		//2D array of all the words in all the documents. Size: MxD
		string** data_list_labeled;
		char*** data_list_unlabeled;
		//Number of documents to read
		int D;
		
		//1D array of the size of each document
		int* documents_size;

		void read_files();
		
};