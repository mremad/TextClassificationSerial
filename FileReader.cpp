#include "FileReader.h"
using namespace std;
FileReader::FileReader(int labeledDocs, int unlabeledDocs)
{
	ConsolePrint::print("File Reader initialized");
	labeled=labeledDocs;
	unlabeled=unlabeledDocs;
	processedDocs=0;
	documents_size= new int[labeledDocs];
	read_files();
}

//Read the content of the document in document_path into the array document_words
void FileReader::read_single_file(string line, string* document_words)
{
	int wordCount=0;
	for(int j=0;j<line.size();j++)
	{
		if(line[j]==' ')
		{
			wordCount++;
		}
		if(line[line.size()-1]==' ')
		{
			wordCount++;
		}
	}
	document_words=new string[wordCount];
	int indexInDoc=0;
	document_words[indexInDoc]="";
	for(int k=0;k<line.size();k++)
	{
		if(line[k]!=' ')
		{
			document_words[indexInDoc].append(line,k,1);
			printf("%s",document_words[indexInDoc]);
		}
		if(line[k]==' ')
		{
			indexInDoc++;
			document_words[indexInDoc]="";
		}
	}
	
	documents_size[processedDocs]=wordCount;
	processedDocs++;
}

//Read all the data set into data_list array
void FileReader::read_files()
{
	string line;
	D=0;
	data_list_labeled=new string*[labeled];
  ifstream myfile ("DocumentClassifierData.txt");
  if (myfile.is_open())
  {
	  int i=0;
    while ( getline (myfile,line)&&i<labeled)
    {
      read_single_file(line,data_list_labeled[i]);
	  D++;i++;
    }
    myfile.close();
  }

  else cout << "Unable to open file";
  printf("%s",data_list_labeled[0][0]);
}

