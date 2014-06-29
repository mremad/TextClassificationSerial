#include "FileReader.h"
using namespace std;
FileReader::FileReader(int docs_to_read, string document_path)
{
	ConsolePrint::print_string("File Reader initialized\n");
	num_docs=docs_to_read;
	path=document_path;
	processed_docs=0;
	documents_size= new int[num_docs];
}

//Read the content of the document in document_path into the array document_words
void FileReader::read_single_file(string line, string* document_words)
{
	int index_in_doc=0;//index for the word in the document array
	document_words[index_in_doc]="";//initialize the string to append the word to it
	for(int k=0;k<line.size();k++)
	{
		if((line[k]==' '|| line[k]==9) && k<line.size()-1)//word ended initialize new string and increase indexInDoc
		{
			index_in_doc++;
			document_words[index_in_doc]="";
		}
		else
		if(line[k]!=' ' || line[k]!=9)
		{
			document_words[index_in_doc].append(line,k,1);
		}
		
	}
}

void FileReader::read_files_per_label(int docs_per_label, int num_labels)
{
    string** new_data_list = (string**)malloc(sizeof(string*)*docs_per_label*num_labels);
    int * new_docs_size = (int*)malloc(sizeof(int)*docs_per_label*num_labels);
    string curr_label = data_list[0][0];
    int curr_new_doc_index = 0;
    int read_docs_per_label = 0;
    
    for(int i = 0;i < num_docs;i++)
    {
        if(read_docs_per_label == docs_per_label && data_list[i][0] == curr_label)
        {
			delete [] data_list[i];
            continue;
        }
        else if(read_docs_per_label == docs_per_label && data_list[i][0] != curr_label)
        {
            read_docs_per_label = 0;
            curr_label = data_list[i][0];
        }
        

        new_data_list[curr_new_doc_index] = data_list[i];
        new_docs_size[curr_new_doc_index] = documents_size[i];
        curr_new_doc_index++;
        read_docs_per_label++;
    }
    
    data_list = new_data_list;

	delete[] documents_size;

    documents_size = new_docs_size;
	num_docs=docs_per_label*num_labels;
}

//free the memory
void FileReader::deallocate()
{
	for(int i=0;i<num_docs;i++)
	{
		delete[] data_list[i];
	}

	delete[] data_list;
	delete[] documents_size;
}
//Read all the data set into data_list array
void FileReader::read_files()
{
	string line;
	data_list = (string**)malloc(sizeof(string*)*num_docs);
	ifstream myfile (path);
	if (myfile.is_open())
	{
		int i=0;
		while ( getline (myfile,line)&&i<num_docs)
		{
			int word_count=0;
			for(int j=0;j<line.size();j++)//count words in the document(line)
			{
				if(line[j]==' '|| line[j]==9)
				{
					word_count++;
				}
			}
			if(line[line.size()-1]!=' ')//in case the document didn't end with a space 
			{							//count for the last word
				word_count++;
			}
			data_list[i]=new string[word_count];//allocate array for each document(line)
			
			read_single_file(line,data_list[i]);
			documents_size[processed_docs]=word_count;
			processed_docs++;
			i++;
		}
		myfile.close();
        printf("%d Documents read successfully\n",num_docs);
	}

  else cout << "Unable to open file\n";
}

