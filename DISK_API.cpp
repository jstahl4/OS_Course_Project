/*
* Lead author: Tom Kueny
* Contributor: James Stahl
*
* Basically implemented Lyon's demo for a disk object to have
* something to work off of for the ATOS API
*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <vector>
#include "Directory.h"

#define SENTINEL 999

using namespace std;
    class BlockType{
    public:
        int blockSize;
        char* data;
        BlockType(int blcksize = 10){
            blockSize = 10;
            data = new char[blcksize];
            for(int i = 0; i < blcksize; i++)
                data[i] = NULL;
        }
        ~BlockType(){ delete data; };
    };
    class Disk{
    private:
        int blockSize;
        int numBlocks;
        int numCreated;
        int numReads;
        int numWrites;
        int currentBlock;
        bool logging;
        Directory directory;
        // std::list<int> freeSpaceList;
        ofstream logfile;
        vector<BlockType*> disk;

        bool createBlock(int blockNumber){
            if(blockNumber < numBlocks){
                disk[blockNumber] = new BlockType(blockSize);
                numCreated++;
                return true;
            }
            else {
                cerr << "DISK: Tried to create a block beyond disk size.\n";
                return false;
            }
        }

    public:

        Disk(int blcksize, int blocknum){
            enableLogging("logfile.txt");
        	blockSize = blcksize;
            numBlocks = blocknum;
            BlockType* block;
            numCreated=numReads=numWrites=currentBlock=0;
            for(int i = 0; i < numBlocks; i++)
                disk.push_back(block);
        }
        ~Disk(){
            for(int i = 0; i < numBlocks; i++)
                if(disk[i] != NULL)
                    delete disk[i];
            if (logging) {
              logfile << "DISK: Terminating.\n";
              logfile << "DISK: Blocks created were "<<numCreated<<" of total "
        	      <<numBlocks<<" which is "
        	      <<100*float(numCreated)/float(numBlocks)<<" %\n";
              logfile.close();
            }
        }

        int getBlockSize() { return blockSize; }
        int getNumBlocks() { return numBlocks; }

        // bool CreateDisk(int blockSize, int numBlocks); Constructor for Disk class?
        bool enableLogging(string logfileName){
            logfile.open(logfileName);
            if (logfile.is_open()) { // did the file open?
              logging = true;
              logfile<<"DISK: Logging enabled to "<<logfileName<<"\n";
              logfile<<"DISK: block size "<<blockSize
                <<" number of blocks "<<numBlocks<<"\n";
            } else cerr<<"DISK: Could not open "<<logfileName<<"\n";
            return logging;
        }
        bool WriteDisk(int offset, BlockType* buffer){
            if (offset<0 || offset>=numBlocks) {
              cerr<<"DISK: Block number is outside bounds\n";
              return -1;
            }
            if (buffer->blockSize != blockSize) {
              cerr<<"DISK: bad block size sent to write.\n";
              return -1;
            }
            if (disk[offset]==NULL) // not ceated yet
              if (!createBlock(offset)) // is it possible to create?
                return -1; // bad block number create request

            // do the data write
            if (buffer->data!=NULL && disk[offset]->data!=NULL) {
              for (int i=0; i<blockSize; i++)
                disk[offset]->data[i] = buffer->data[i];
              numWrites++;
            }
            else cerr<<"DISK: Bad buffer pointers sent to write\n";

            // do logging if enabled
            if (logging)
              logfile<<"DISK: Write to block "<<offset<<"\n";
            return 0;
        }
        bool ReadDisk(int offset, BlockType* buffer){
            if(offset<0 || offset>=numBlocks){
                cerr << "DISK: Block number is outside bounds\n";
                return -1;
            }
            if (buffer->blockSize != blockSize) {
              cerr<<"DISK: Bad block size sent to read.\n";
              return -1;
            }
            if (disk[offset]==NULL)
                if (!createBlock(offset))
                    return -1;
            if (buffer->data!=NULL && disk[offset]->data!=NULL) {
              for (int i=0; i<blockSize; i++)
                buffer->data[i] = disk[offset]->data[i];
              numReads++;
            }
            else cerr<<"DISK: Bad buffer pointers sent to read\n";
            if (logging)
              logfile <<"DISK: Read to block "<< offset <<"\n";
            return 0;
        }
        void writeStats(){
            logfile<< "DISK: block size "<<blockSize
               << " number of blocks "<<numBlocks<<"\n";
            logfile << "DISK: Num reads "<<numReads
                << " Num writes "<<numWrites<<"\n";
            logfile << "DISK: Blocks created were "<<numCreated<<" of total "
                << numBlocks<<" which is "
                << 100*float(numCreated)/float(numBlocks)<<" %\n";

            return;
        }
        int availableContiguousBlocks(int blocks){
            int available = 0;
            int curr_max_available = 0;
            for(int i=0; i < numBlocks; i++){
                if (disk[i]->data == NULL)
                    curr_max_available++;
                if(curr_max_available > available)
                    available = curr_max_available;
                if(disk[i]->data != NULL)
                    curr_max_available = 0;
            }
            return available;
        }
        int availableContiguousBlocksStartBlock(int blockSize){
            int startBlock = 0;
            int counter = 0;
            bool goodBlockFound = false;
            for(int i = 0; i < numBlocks; i++){
                if(i == 0){
                    if(disk[0]->data == NULL){
                        startBlock = 0;
                        for(int j = 0; j < blockSize+1; j++){
                            if(disk[j]->data == NULL){
                                counter++;
                            }
                            if(counter >= blockSize){
                                goodBlockFound = true;
                                return startBlock;
                            }
                        }
                    }
                    else if(disk[i - 1]->data != NULL && disk[i]->data == NULL){
                        for(int j = 0; j < blockSize+1; j++){
                            if(disk[j]->data == NULL){
                                counter++;
                            }
                            if(counter >= blockSize){
                                goodBlockFound = true;
                                return startBlock;
                            }
                        }
                    }
                }
            }
            if(!goodBlockFound){
                cerr << "Not enough space in disk for file of this block size (" << blockSize << ").\n";
                return -1;
            }
            return -1;
        }
        bool compactionNeeded(){
            bool needed = false;
            bool emptyBlockFound = false;
            int emptyStartBlock;
            for(int i = 0; i < numBlocks; i++){
                if(emptyBlockFound && disk[i]->data != NULL)
                    needed = true;
                    break;
                if(disk[i]->data == NULL)
                    emptyBlockFound = true;
            }
            return needed;
        }
        void compact(){
            //base case
            if(compactionNeeded())
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
                        nextFileIndex = ctr;
                    }
                    ctr++;
                }
                //copy file object to an object NOT ON THE DISK
                File newFile = directory.get_File(nextFileIndex);
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

    bool  Create(std::string const& aFileName, int fileSize = 0) {
              cout << "1\n";
			  File newFile(aFileName);
              cout << "2\n";
			  directory.add_file(newFile);
              cout << "3\n";
              return true; //add file to set, no size = no writing, yet
	}
	bool	Delete(std::string const& aFileName) {

				//NOTE: COMPACT MUST BE CALLED IN TANDEM WITH DELETE
				File target = directory.get_File(aFileName);
				BlockType* NULLBLOCK;
                char* nullBuffer = NULL;
                NULLBLOCK->data = nullBuffer;
                int fileBlocks = target.get_block_size();
				int startingBlock = target.get_starting_block();
				for(int i = startingBlock; i < startingBlock + fileBlocks; i++){
					WriteDisk(i, NULLBLOCK);
				}
				directory.remove_file(aFileName);
	}
	File	Open(std::string const& aFileName) {
			return directory.get_File(aFileName);
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
	int		Write(File &obj, char* newBuffer, int numchards = 0){
			//no idea why numchards is passed but hey lets roll with it
			numchards = 0;
			//get amount of chars
			while(newBuffer[numchards] != '\0')
			{
				numchards++;
			}
			//calculate number of blocks
			int numberofBlocks = (int)ceil(numchards/blockSize /*10*/);
			//set file obj to have correct block size based on buffer
			obj.set_block_size(numberofBlocks);
			//check to see if there's enough room on the disk
			int avail = availableContiguousBlocks(obj.get_block_size());
			//if enough space, set a starting block
            int startingBlock = SENTINEL;    // initialized to SENTINEL in case no blocks available
			if(avail >= numberofBlocks){
				startingBlock =  availableContiguousBlocksStartBlock(obj.get_block_size());
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
                BlockType* newBlock;
				for(int y = 0; y < blockSize && fileBufferIndex < numchards; y++){
					newBlock->data[y] = fileBuffer[fileBufferIndex];
					fileBufferIndex++;
				}
				WriteDisk(obj.get_starting_block() + x, newBlock);
			}
			return numchards;
	}

	int		Stats(std::string const& /*aFileName*/) { return 5; };
	std::vector<std::string> List() { std::vector<std::string> res; res.push_back("asapasasdFile1"); return res; }
    };

int main(){
    Disk d(10,10);
    string fileName = "TESTFILE.txt";
    cout << "Disk successfully created \n";
    cout << "Creating file named " << fileName << " ...\n";
    d.Create(fileName);
    File newFile = d.Open(fileName);
    cout << "File named " << newFile.get_name() << " was successfully created.\n";
    char* buffer = (char *) "Hi I'm writing data to the file.";
    newFile.set_data(buffer);
    d.Write(newFile, buffer);
    cout << newFile.data << endl;
    cout << newFile.get_name() << " was written to successfully\n";
 }
