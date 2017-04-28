/**********************************************************************
 * James Stahl - Team Coyote
 ***********************************************************************
 *
 * File: directory.cpp
 * Defines a directory system
 *
 * Implementation: linear list, with a hash function to determine where in the list
 * files are stored.
 */

#include<iostream>
#include "directory.h"
#include "diskprocess.h"

// constructors
directory::directory()
{

}

directory::directory(DiskProcessType process)
{
	
}

// accessors
string directory::get_file_name()
{
	return file_name;
}

// mutators
void directory::add_file(string f, int s, DiskBlockType & * block)
{
	// create new node, add it to the list, then switch to new node
	current->next = new node;
	current->next->prev = current;
	current = current->next;

	// set values in current node
	current->file_name = f;
	current->size = s;
	current->file = block;

	// add current node to unordered_set (for easy access via hashing)
	nodes.insert(*current);
}
