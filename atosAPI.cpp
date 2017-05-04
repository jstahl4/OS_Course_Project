/**********************************************************************
 * Tom Kueny
 **********************************************************************/
#include "File.h"
#include "Directory.h"
using namespace DISK_API;
namespace ATOS_API
{
	// int fname[10],fsize[10],fstart[10],freest[10],freesize[10],m=0,n=0,start;
	bool  Create(std::string const& aFileName, int fileSize) {
			  File newFile(aFileName, fileSize);
			  add_file(newFile); //add file to set, no size = no writing, yet
			  //below is probably better for write method
		// 	  int i,flag=1,j,a;
		// 	  for(i=0;i<=m;i++){
		// 	 		if( freesize[i] >= size)
		// 				a=i,flag=0;
		// 	  if(!flag){
		// 	  	for(j=0;j<n;j++);
		// 		n++;
		// 	  	fname[j]=name;
		// 	  	fsize[j]=size;
		// 	  	fstart[j]=freest[a];
		// 	  	freest[a]=freest[a]+size;
		// 		freesize[a]=freesize[a]-size;
		   //
		// 		cout << "\n The memory map will now be: \n\n";
		// 	  	display();
		// 	  }
		// 	  else
		// 	  {
		// 		cout << "\nNo enough space is available.System compaction......\n";
		   //
		// 		flag=1;
		// 		compaction();
		// 		display();
		// 	 	for(i=0;i<=m;i++)
		// 			if( freesize[i] >= size)
		// 		 a=i,flag=0;
		// 	  if(!flag)
		// 	   {
		// 		for(j=0;j<n;j++);
		// 		   n++;
		// 		fname[j]=name;
		// 		fsize[j]=size;
		// 		fstart[j]=freest[a];
		// 		freest[a]+=size;
		// 		freesize[a]-=size;
		// 		cout << "\n The memory map will now be: \n\n";
		// 		display();
		// 	   }
		// 	   else
		// 	  	cout << "\nNo enough space.\n";
		// 	   }
		//    }

	}
	bool	Delete(std::string const& aFileName*) {

				//TODO: need a file accessor to get the object
				File target = get_File(aFileName);
				char* nullBuffer[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
				int fileBlocks = target.get_block_size();
				int startingBlock = target.get_starting_block();
				for(int i = startingBlock; i < startingBlock + fileBlocks; i++){
					WriteDisk(i, nullBuffer);
				}
				remove_file(aFileName);
				// if(compactionNeeded())
				// 	compact();
	}
	int		Open(std::string const& /*aFileName*/) {
			//find file (search algorithm?)
			//if file is not in disk, throw error
			//if file is in disk -> open file and allow for read/write
			//return 1 if successful, else
			return 0;
	} // 0 is failed.
	bool	Close(int /*aHandle*/) {
			//close file
			//return pointer to initial position
			return true;
	}
	int		Read(int /*handle*/, int /*numchards*/, char* /*buffer*/) {
			//pass data to buffer
			//increment numchars
			//handle?
			return 0;
	}
	int		Write(File &obj, char const* newBuffer, int numchards = 0){
			numchards = 0;
			while(newBuffer[numchards] != '\0')
			{
				numchards++;
			}
			int numberofBlocks = ceil(numchards/blockSize /*10*/);
			obj.set_block_size(numberofBlocks)
			int avail = availabeContiguousBlocks(obj.get_block_size());
			if(avail >= numberofBlocks){
				int startingBlock =  availableContiguousBlocksStartBlock(obj.get_block_size());
			}
			obj.set_starting_block(startingBlock);
			obj.set_data(newBuffer);
			char* fileBuffer = obj.get_data();
			static int fileBufferIndex = 0;
			for(int x = 0; x < obj.get_block_size(); x++)
			{
				char * blockBuffer[10];
				for(int y = 0; y < blockSize; y++){
					blockBuffer =  fileBuffer[fileBufferIndex];
					fileBufferIndex++;
				}
				WriteDisk(obj.get_starting_block() + x, blockBuffer);
			}
			return numchards;
	}

	int		Stats(std::string const& /*aFileName*/) { return 5; };
	std::vector<std::string> List() { std::vector<std::string> res; res.push_back("asapasasdFile1"); return res; }

	void compact(){
		if(!compactionNeeded())
			return;
		else if(compactionNeeded()){

			bool empty = false;
			int n = 0;
			int firstEmptyIndex;
			while(!empty){
				if(disk[n]->data == NULL){
					empty = true;
					firstEmptyIndex = n;
				}
				n++;
			}

			bool nextFileFound = false;
			int nextFileIndex;
			int ctr = firstEmptyIndex;
			while(!nextFileFound)
			{
				if(disk[ctr]->data != NULL){
					nextFileFound = true;
					newFileIndex = ctr;
				}
				ctr++;
			}
			File newFile = getFileByIndex(newFileIndex);
			Delete(newFile.get_name());
			newFile.set_starting_block(firstEmptyIndex);
			Write(newFile, newFile.get_data());
			if(compactionNeeded())
				compact();
		}
	}
	// void    compaction(){
	// 		    int i,j,size1=0,f_size=0;
	// 		      if(fstart[0]!=start)
	// 		       {
	// 		    fstart[0]=start;
	// 		    for(i=1;i<n;i++)
	// 		      fstart[i]=fstart[i-1]+fsize[i-1];
	// 		      }
	// 		     else
	// 		       {
	// 		      for(i=1;i<n;i++)
	// 		       fstart[i]=fstart[i-1]+fsize[i-1];
	// 		       }
	// 		     f_size=freesize[0];
	//
	// 		     for(j=0;j<=m;j++)
	// 		       size1+=freesize[j];
	// 		      freest[0]=freest[0]-(size1-f_size);
	// 		      freesize[0]=size1;
	// 		      m=0;
	// }
	// void 	display(){
	//      int i;
	//
	//      cout << "\n  ***   MEMORY MAP TABLE  ***        \n";
	//      cout << "\n\nNAME     SIZE    STARTING ADDRESS      \n\n";
	//      for(i=0;i<n;i++)
	//        cout << fname[i] << "\t\t" << fsize[i] << "\t\t" << fstart[i] << endl;
	//      cout << "\n\n";
	//      cout << "\n\n***  FREE SPACE TABLE  ***\n\n";
	//      cout << "FREE START ADDRESS             FREE SIZE        \n\n";
	//      for(i=0;i<=m;i++)
	//      cout << freest[i] << "\t\t" << freesize[i] << endl;
	//    }
}
