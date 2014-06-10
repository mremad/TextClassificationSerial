#include "FileReader.h"
using namespace std;
FileReader::FileReader(int labeledDocs, int unlabeledDocs)
{
	ConsolePrint::print("File Reader initialized\n");
	labeled=labeledDocs;
	unlabeled=unlabeledDocs;
	processedDocs=0;
	documents_size= new int[labeledDocs];
	read_files();
}

//Read the content of the document in document_path into the array document_words
void FileReader::read_single_file(string line, string* document_words)
{
	int indexInDoc=0;
	document_words[indexInDoc]="";
	for(int k=0;k<line.size();k++)
	{
		if(line[k]==' '|| line[k]==9)
		{
			indexInDoc++;
			document_words[indexInDoc]="";
		}
		else
		if(line[k]!=' ' || line[k]!=9)
		{
			document_words[indexInDoc].append(line,k,1);
		}
		
	}
}

//Read all the data set into data_list array
void FileReader::read_files()
{
	string line;
	D=0;
	data_list_labeled=new string*[labeled];
  ifstream myfile ("/Users/Mohamed/Desktop/TextClassificationSerial/DocumentClassifierData.txt");
  if (myfile.is_open())
  {
	  int i=0;
    while ( getline (myfile,line)&&i<labeled)
    {
	int wordCount=0;
	for(int j=0;j<line.size();j++)
	{
		if(line[j]==' '|| line[j]==9)
		{
			wordCount++;
		}
	}
	if(line[line.size()-1]!=' ')
		{
			wordCount++;
		}
		data_list_labeled[i]=new string[wordCount];
		read_single_file(line,data_list_labeled[i]);
		documents_size[processedDocs]=wordCount;
		processedDocs++;
		D++;i++;
    }
    myfile.close();
  }

  else cout << "Unable to open file";
}

