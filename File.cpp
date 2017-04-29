#include "file.h"
#define DEFAULT_BLOCK_SIZE 10

// set size of 1 char in bytes (i.e. if CHAR_SIZE is 1, then 1 char = 1 byte)
#define CHAR_SIZE 1

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

// destructor
File::~File()
{
	delete data;
}

// accessors
string File::get_name()
{
	return name;
}
int File::get_size()
{
	return size;
}
char * File::get_data()
{
	return data;
}

// mutators
void File::set_data(char * d)
{
	data = d;
}
void File::set_size(int s)
{
	size = s;
}
static void File:::set_block_size(int s)
{
	block_size = s;
}
void File::set_name(string n)
{
	name = n;
}

// calculators

/* calculates size in blocks and sets size accordingly */
void File::calculate_size()
{
	// determine size of data in bytes
	string d = data;
	int bytes = CHAR_SIZE * d.length();
	
	// determine size of data in blocks
	int blocks = bytes / block_size;

	// set size accordingly
	size = blocks;
}
