
class FileReader
{
	public:
		FileReader();
		//2D array of all the words in all the documents. Size: MxD
		char*** data_list_labeled;
		char*** data_list_unlabeled;
		//Number of documents to read
		int D;
		void read_files();
		
}