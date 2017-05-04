/**********************************************************************
 * Tom Kueny
 **********************************************************************/
#include "File.h"
#include "Directory.h"
using namespace DISK_API;
namespace ATOS_API
{
	bool  Create(std::string const& aFileName, int fileSize) {
			  File newFile(aFileName, fileSize);
			  add_file(newFile); //add file to set, no size = no writing, yet
	}
	bool	Delete(std::string const& aFileName*) {

				//NOTE: COMPACT MUST BE CALLED IN TANDEM WITH DELETE
				File target = get_File(aFileName);
				char* nullBuffer[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
				int fileBlocks = target.get_block_size();
				int startingBlock = target.get_starting_block();
				for(int i = startingBlock; i < startingBlock + fileBlocks; i++){
					WriteDisk(i, nullBuffer);
				}
				remove_file(aFileName);
	}
	File&	Open(std::string const& aFileName) {
			return get_File(aFileName);
	}
	bool	Close(int ) {
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
			//no idea why numchards is passed but hey lets roll with it
			numchards = 0;
			//get amount of chars
			while(newBuffer[numchards] != '\0')
			{
				numchards++;
			}
			//calculate number of blocks
			int numberofBlocks = ceil(numchards/blockSize /*10*/);
			//set file obj to have correct block size based on buffer
			obj.set_block_size(numberofBlocks)
			//check to see if there's enough room on the disk
			int avail = availabeContiguousBlocks(obj.get_block_size());
			//if enough space, set a starting block
			if(avail >= numberofBlocks){
				int startingBlock =  availableContiguousBlocksStartBlock(obj.get_block_size());
			}
			obj.set_starting_block(startingBlock);
			//set file data buffer
			obj.set_data(newBuffer);
			//copy data buffer
			char* fileBuffer = obj.get_data();
			//counter for buffer
			static int fileBufferIndex = 0;
			for(int x = 0; x < obj.get_block_size(); x++)
			{
				char * blockBuffer[10];
				for(int y = 0; y < blockSize && fileBufferIndex < numchards; y++){
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
		//base case
		if(!compactionNeeded())
			return;
		else if(compactionNeeded()){
			//get empty block index
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
			//get next file's index
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
			//copy file object to an object NOT ON THE DISK
			File newFile = get_File(newFileIndex);
			//delete object with same name ON THE DISK
			Delete(newFile.get_name());
			//write copied file on disk at appropriate index
			newFile.set_starting_block(firstEmptyIndex);
			Write(newFile, newFile.get_data());
			//check for more files that need to be compacted
			if(compactionNeeded())
				compact();
		}
	}
}
