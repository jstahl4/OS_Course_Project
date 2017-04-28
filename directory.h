/**********************************************************************
 * James Stahl - Team Coyote
 **********************************************************************

 * File: directory.h
 * Defines a directory system
 *
 * Implementation: files stored in linear list and accessed via hash 
 * function.
 */

#include <iostream>
#include <vector>
using namespace std;

struct node 
{
	// name of file
	string file_name;

	// first block in the file
	DiskBlockType * start;

	// size of file in blocks
	int size;
}

/* hash functionality */
struct node_hasher
{
	// overload operator (), so this can be called as a function
	size_t operator()(const node & n) const
	{
		// returns an unsigned int using stl-provided hash function
		return hash<string>()(n.file_name);
	}
};

class directory
{
	public:
		// constructors
		directory();
		directory(DiskProcessType);

		// accessors
		void list();

		// mutators
		void add_file(string, int, DiskBlockType & *);
		
		// init
		void init;
		
	private:
		vector<node> files;
};
