/**********************************************************************
 * James Stahl - Team Coyote
 **********************************************************************

 * File: directory.h
 * Defines a directory system
 *
 * Implementation: files stored in unordered_set and accessed through hash function.
 * Doubly linked list of files also implemented.
 */

#include <iostream>
#include <unordered_set>
using namespace std;

/* nodes to represent files */
struct node
{
	// name of file
	string file_name;
	
	// size in blocks
	int size;

	// pointer to first block
	DiskBlockType * file;
	
	// pointers to other list elements
	node * next;
	node * prev;

};

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

		// accessors
		node * get_head();
		
		node * get_current();

		// mutators
		void add_file(string, int, DiskBlockType & *);
		
		
	private:
		unordered_set<node, node_hasher> nodes;
		node * head;
		node * current;
};
