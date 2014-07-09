#include "FileReader.h"
#include "Cfg.h"

using namespace std;
const int FIXED_STRING_SIZE=25;
FileReader::FileReader(int docs_to_read, string document_path)
{
	ConsolePrint::print_string("File Reader initialized\n");
	num_docs=docs_to_read;
	path=document_path;
	processed_docs=0;
	documents_size= new int[num_docs];
}


bool FileReader::check_if_feature(string word)
{
    bool check = true;
    
	if(word.length() <= 3 || word.length()> (HASH_WORD_SIZE-1))
    {
        check = false;
    }
    else if(word == "though" || word == "they" || word == "that" || word == "this" || word == "there" || word == "were"
            || word == "than" || word == "rather" || word == "from" || word == "most")
        check = false;
    
    return check;
}

//Read the content of the document in document_path into the array document_words
void FileReader::read_single_file(string line, string* document_words,int word_count)
{
	bool label_string=true;
	int index_in_doc=0;//index for the word in the document array
	document_words[index_in_doc]="";//initialize the string to append the word to it
	for(int k=0;k<line.size();k++)
	{
		if((line[k]==' '|| line[k]==9) && k<line.size()-1 
			&& (label_string 
			|| check_if_feature(document_words[index_in_doc])))//word ended initialize new string and increase indexInDoc
		{
			label_string=false;
			index_in_doc++;
			if(index_in_doc<word_count)
			{
				document_words[index_in_doc]="";
			}
			else
				break;
		}
		else
		if(line[k]!=' ' && line[k]!=9)
		{
			document_words[index_in_doc].append(line,k,1);
		}
		else
		if((line[k]==' '|| line[k]==9) && !check_if_feature(document_words[index_in_doc]))
			{
				document_words[index_in_doc]="";
			}
	}
	
}



void FileReader::label_factory(int desired_labeled, int desired_unlabeled, int num_labels, string** Udata_out, string** Ldata_out)
{
	string last_label="";
	int docs_per_label = desired_labeled/num_labels;
	int docs_per_unlabel = desired_unlabeled/num_labels;
	int curr_docs_in_label=0;
	int curr_docs_in_unlabel=0;
	desired_labeled--;
	desired_unlabeled--;
	documents_size_labeled= new int[desired_labeled];
	documents_size_unlabeled= new int[desired_unlabeled];
	for(int i=0;i<num_docs && (desired_labeled+desired_unlabeled>=-1);i++)
	{
		if(data_list[i][0].compare(last_label)!=0)
		{
			Ldata_out[desired_labeled]=data_list[i];
			documents_size_labeled[desired_labeled] = documents_size[i];
			//printf("Labeled %d = %s %s\n",desired_labeled,data_list[i][0].c_str(),data_list[i][1].c_str());
			desired_labeled--;
			last_label = data_list[i][0];
			curr_docs_in_label = 1;
			curr_docs_in_unlabel=0;
		}
		else if(data_list[i][0].compare(last_label)==0 && curr_docs_in_label<docs_per_label)
		{
			Ldata_out[desired_labeled]=data_list[i];
			documents_size_labeled[desired_labeled] = documents_size[i];
			//printf("Labeled %d = %s %s\n",desired_labeled,data_list[i][0].c_str(),data_list[i][1].c_str());
			desired_labeled--;
			curr_docs_in_label++;
		}
		else if(data_list[i][0].compare(last_label)==0 && desired_unlabeled>=0 && curr_docs_in_unlabel<docs_per_unlabel)
		{
			Udata_out[desired_unlabeled]=data_list[i];
			documents_size_unlabeled[desired_unlabeled] = documents_size[i];
			//printf("UnLabeled %d = %s %s\n",desired_unlabeled,data_list[i][0].c_str(),data_list[i][1].c_str());
			desired_unlabeled--;
			curr_docs_in_unlabel++;
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
	int processed_docs_counter=0;
	for(int i=0;i<num_docs;i++)
	{
			delete[] data_list[i];
	}

	delete[] data_list;

}
//Read all the data set into data_list array
void FileReader::read_files()
{
	string line;
	data_list = (string**)malloc(sizeof(string*)*num_docs);
	ifstream myfile (path.c_str());
	if (myfile.is_open())
	{
		int i=0;
		int word_count=0;
		int curr_string_length=0;
		bool label_string=true;
		total_char_count=0;
		total_word_count=0;
		while ( getline (myfile,line)&&i<num_docs)
		{	
		
			word_count=0;
			curr_string_length=0;
			label_string=true;
			string curr_word="";
			for(int j=0;j<line.size();j++)//count words in the document(line)
			{
				if(line[j]==' '|| line[j]==9)
				{
					if(label_string || check_if_feature(curr_word))
					   {
						   label_string=false;
						   word_count++;
						   total_char_count+=curr_string_length;
					   }
						curr_word="";
						curr_string_length=-1;
				}
				if(curr_string_length!=-1)
					curr_word.append(line,j,1);
				curr_string_length++;
			}
			if(line[line.size()-1]!=' ')//in case the document didn't end with a space 
			{
			   if(check_if_feature(curr_word))	//count for the last word
				{
					word_count++;
					total_char_count+=curr_string_length;
			   }
			}
			data_list[i]=new string[word_count];//allocate array for each document(line)
			total_word_count+=word_count;
			read_single_file(line,data_list[i],word_count);
			documents_size[processed_docs]=word_count;
			processed_docs++;
			i++;
		}
		myfile.close();
        printf("%d Documents read successfully\n",num_docs);
	}

  else cout << "Unable to open file\n";
}

