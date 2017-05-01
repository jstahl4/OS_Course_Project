/*
*Tom Kueny
*
*Basically implemented Lyon's demo for a disk object to have
*something to work off of for the ATOS API
*/
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;
namespace DISK_API{
    class BlockType{
    public:
        int blockSize;
        char* data;
        bool isWrittenTo;
        BlockType(int blcksize){
            blockSize = blcksize;
            isWrittenTo = false;
            data = new char[blcksize];
            for(int i = 0; i < blcksize; i++)
                data[i] = '.';
        }
        ~BlockType(){ delete data; };
        void blockWrite(){
            isWrittenTo = true;
        }
        void blockErase(){
            isWrittenTo = false;
        }
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
        std::list<int> freeSpaceList;
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
            numCreated=numReads=numWrites=currentBlock=0;
            for(int i = 0; i < numBlocks; i++)
                disk.push_back(NULL);
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
        bool needsCompaction(unordered_set<File, file_hasher> fileList){
            //TODO: iterate through list, if numblocks created > numblocks taken by files
            //return true, else false
        }
        void compaction(){
            //locate first free block
            //assert a file exists after this block
            //find next file
            //create new file object as a copy
            //deallocate blocks aka block->data == NULL
            //find aforementioned lowest block
            //write copied file with that block as the offset
            //repeat until contiguous space is compacted
        }

    };

}
