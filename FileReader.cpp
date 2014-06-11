#include "FileReader.h"
using namespace std;
FileReader::FileReader(int docs_to_read, string document_path)
{
	ConsolePrint::print("File Reader initialized\n");
	num_docs=docs_to_read;
	path=document_path;
	processed_docs=0;
	documents_size= new int[num_docs];
	read_files();
}

//Read the content of the document in document_path into the array document_words
void FileReader::read_single_file(string line, string* document_words)
{
	int index_in_doc=0;//index for the word in the document array
	document_words[index_in_doc]="";//initialize the string to append the word to it
	for(int k=0;k<line.size();k++)
	{
		if(line[k]==' '|| line[k]==9)//word ended initialize new string and increase indexInDoc
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

//Read all the data set into data_list array
void FileReader::read_files()
{
	string line;
	data_list=new string*[num_docs];
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
	}

  else cout << "Unable to open file\n";
}

