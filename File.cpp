#include "file.h"
#define DEFAULT_BLOCK_SIZE 10

int File::block_size = DEFAULT_BLOCK_SIZE;

File::File(std::string fileName){
	data=NULL;
	size=0;
	name = fileName;
}
File::File(std::string fileName, int blockSize, char* buffer = NULL){
	data=buffer;
	size=blockSize;
	name=fileName;
}
