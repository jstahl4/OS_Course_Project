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

class BlockType
{
public:
    int blockSize;
    char *data;

    BlockType(int blcksize = 10)
    {
        blockSize = 10;
        data = new char[blcksize];
        for (int i = 0; i < blcksize; i++)
            data[i] = '\0';
    }

    ~BlockType()
    { delete data; };
};

class Disk
{
//    private:
public:
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
    vector<BlockType *> disk;

    bool createBlock(int blockNumber)
    {
        if (blockNumber < numBlocks)
        {
            // push_back() used to add BlockTypes to disk
            disk.push_back(new BlockType(blockSize));
            numCreated++;
            return true;
        } else
        {
            cerr << "DISK: Tried to create a block beyond disk size.\n";
            return false;
        }
    }

public:

    Disk(int blcksize, int blocknum)
    {
        enableLogging("logfile.txt");
        blockSize = blcksize;
        numBlocks = blocknum;
        BlockType *block = new BlockType();
        numCreated = numReads = numWrites = currentBlock = 0;
        for (int i = 0; i < numBlocks; i++)
            createBlock(i);
        disk.push_back(block);
    }

    ~Disk()
    {
        for (int i = 0; i < numBlocks; i++)
            if (disk[i] != NULL)
                delete disk[i];
        if (logging)
        {
            logfile << "DISK: Terminating.\n";
            logfile << "DISK: Blocks created were " << numCreated << " of total "
                    << numBlocks << " which is "
                    << 100 * float(numCreated) / float(numBlocks) << " %\n";
            logfile.close();
        }
    }

    int getBlockSize()
    { return blockSize; }

    int getNumBlocks()
    { return numBlocks; }

    // bool CreateDisk(int blockSize, int numBlocks); Constructor for Disk class?
    bool enableLogging(string logfileName)
    {
        logfile.open(logfileName);
        if (logfile.is_open())
        { // did the file open?
            logging = true;
            logfile << "DISK: Logging enabled to " << logfileName << "\n";
            logfile << "DISK: block size " << blockSize
                    << " number of blocks " << numBlocks << "\n";
        } else cerr << "DISK: Could not open " << logfileName << "\n";
        return logging;
    }

    bool WriteDisk(int offset, BlockType *buffer)
    {


        if (offset < 0 || offset >= numBlocks)
        {
            cerr << "DISK: Block number is outside bounds\n";


            return -1;
        }
        if (buffer->blockSize != blockSize)
        {
            cerr << "DISK: bad block size sent to write.\n";
            return -1;
        }
        if (disk[offset] == NULL) // not ceated yet
            if (!createBlock(offset)) // is it possible to create?
                return -1; // bad block number create request

        // do the data write
        if (buffer->data != NULL && disk[offset]->data != NULL)
        {
            for (int i = 0; i < blockSize; i++)

                disk[offset]->data[i] = buffer->data[i];
            numWrites++;
        } else cerr << "DISK: Bad buffer pointers sent to write\n";

        // do logging if enabled
        if (logging)
            logfile << "DISK: Write to block " << offset << "\n";
        return 0;

    }

    bool ReadDisk(int offset, BlockType *buffer)
    {
        if (offset < 0 || offset >= numBlocks)
        {
            cerr << "DISK: Block number is outside bounds\n";
            return -1;
        }
        if (buffer->blockSize != blockSize)
        {
            cerr << "DISK: Bad block size sent to read.\n";
            return -1;
        }
        if (disk[offset] == NULL)
            if (!createBlock(offset))
                return -1;
        if (buffer->data != NULL && disk[offset]->data != NULL)
        {
            for (int i = 0; i < blockSize; i++)
                buffer->data[i] = disk[offset]->data[i];
            numReads++;
        } else cerr << "DISK: Bad buffer pointers sent to read\n";
        if (logging)
            logfile << "DISK: Read to block " << offset << "\n";
        return 0;
    }

    void writeStats()
    {
        logfile << "DISK: block size " << blockSize
                << " number of blocks " << numBlocks << "\n";
        logfile << "DISK: Num reads " << numReads
                << " Num writes " << numWrites << "\n";
        logfile << "DISK: Blocks created were " << numCreated << " of total "
                << numBlocks << " which is "
                << 100 * float(numCreated) / float(numBlocks) << " %\n";

        return;
    }

    int availableContiguousBlocksStartBlock(int blockSize) {
        int startingBlock = 0;
        for (int i = 0; i < numBlocks; i++) {

            if (disk[i]->data[0] == '\0') {
                startingBlock = i;

                for (int j = i; j < startingBlock + blockSize; j++) {
                    if (disk[j]->data[0] != '\0') {
                        startingBlock = -1;
                        break;
                    }
                    return startingBlock;
                }
                if (startingBlock != -1)
                    return startingBlock;
            }
        }
        return startingBlock;
    }

    bool compactionNeeded()
    {
        bool needed = false;
        bool emptyBlockFound = false;
        int emptyStartBlock;
        for (int i = 0; i < numBlocks; i++)
        {
            if (emptyBlockFound && disk[i]->data != NULL)
            {
                needed = true;
                break;
            }
            if (disk[i]->data == NULL)
            {
                emptyBlockFound = true;
            }
        }
        return needed;
    }

    void compact()
    {
        //base case
        if (compactionNeeded())
            return;
        else if (compactionNeeded())
        {
            //get empty block index
            bool empty = false;
            int n = 0;
            int firstEmptyIndex;
            while (!empty)
            {
                if (disk[n]->data == NULL)
                {
                    empty = true;
                    firstEmptyIndex = n;
                }
                n++;
            }
            //get next file's index
            bool nextFileFound = false;
            int nextFileIndex;
            int ctr = firstEmptyIndex;
            while (!nextFileFound)
            {
                if (disk[ctr]->data != NULL)
                {
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
            if (compactionNeeded())
                compact();
        }
    }

    bool Create(std::string const &aFileName, int fileSize = 0)
    {
        File newFile(aFileName);
        directory.add_file(newFile);
        return true; //add file to set, no size = no writing, yet
    }

    bool Delete(std::string const &aFileName)
    {
        //NOTE: COMPACT MUST BE CALLED IN TANDEM WITH DELETE

        // obtain info for deletion
        File target = directory.get_File(aFileName);
        int fileBlocks = target.get_size();
        int startingBlock = target.get_starting_block();

        // clear appropriate data on disk and replace with dummy data
        for (int i = startingBlock; i < startingBlock + fileBlocks; i++)
        {
            delete disk[i]->data;
            disk[i]->data = new char[blockSize];
        }

        // remove file from directory
        directory.remove_file(aFileName);
        return true;
    }

    File Open(std::string const &aFileName)
    {
        return directory.get_File(aFileName);
    }

    bool Close(string fileName, char *buffer = NULL)
    {
        //buffer is NULL if no write is done
        if (buffer != NULL)
        {
            File obj = directory.get_File(fileName);
            Write(obj, buffer);
        }
        return true;
    }

    char *Read(string fileName, int numchards = 0, char *buffer = NULL)
    {
        File fileObj = directory.get_File(fileName);
        int starter = fileObj.get_starting_block();
        char *newBuffer;
        static int n = 0;
        for (int i = starter; i < starter + fileObj.get_block_size(); i++)
        {
            int j = 0;
            while (disk[i]->data[j] != '\0' || j < 10) {
                newBuffer[n] = disk[i]->data[j];
                j++;
                n++;
            }
        }

        return newBuffer;
    }

    int Write(File &obj, char *newBuffer, int numchards = 0)
    {
        //no idea why numchards is passed but hey lets roll with it
        numchards = 0;

        //get amount of chars
        while (newBuffer[numchards] != '\0')
        {
            numchards++;
        }

        //calculate number of blocks
        int numberofBlocks = (int) ceil((float) numchards / (float) blockSize /*10*/);

        //set file obj to have correct block size based on buffer
        obj.set_size(numberofBlocks);

        //check to see if there's enough room on the disk

        //if enough space, set a starting block
        int startingBlock;// = SENTINEL;    // initialized to SENTINEL in case no blocks available

        startingBlock = availableContiguousBlocksStartBlock(numberofBlocks);

        if(startingBlock == -1)
        {
            // if not enough blocks
            return -1;
        }

        obj.set_starting_block(startingBlock);

        //set file data buffer
        obj.set_data(newBuffer);

        //copy data buffer
        char *fileBuffer = obj.get_data();

        //counter for buffer
        static int fileBufferIndex = 0;

        int starter = obj.get_starting_block();
        fileBufferIndex = 0;
        for (int x = 0; x < obj.get_block_size(); x++)
        {
            BlockType *newBlock = new BlockType();
            for (int y = 0; y < blockSize && fileBufferIndex < numchards; y++)
            {
                newBlock->data[y] = fileBuffer[fileBufferIndex];
                fileBufferIndex++;
            }

            WriteDisk(starter + x, newBlock);
        }

        /* add file to the directory */

        // remove the file with the given filename
        directory.remove_file(obj);

        // add file back with new info
        directory.add_file(obj);

        return numchards;
    }

    int Stats(std::string const & /*aFileName*/)
    { return 5; };

    std::vector<std::string> List()
    {
        std::vector<std::string> res;
        res.push_back("asapasasdFile1");
        return res;
    }
};

