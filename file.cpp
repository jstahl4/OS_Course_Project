#include "file.h"
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
