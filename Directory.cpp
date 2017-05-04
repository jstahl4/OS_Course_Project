/**********************************************************************
 * James Stahl - Team Coyote
 ***********************************************************************
 *
 * File: Directory.cpp
 * Defines a directory system
 *
 * Implementation: linear list, with a hash function to determine where in the list
 * files are stored.
 */

#include<iostream>
#include "Directory.h"

// constructors
Directory::Directory()
{

}

// accessors/display functions
void Directory::display_list()
{
	for(const auto & f: files)
	{
		cout << f.get_name() << endl;
	}
}

unordered_set<File, file_hasher, file_comparator> Directory::get_file_list()
{
	return files;
}

string * Directory::get_file_name_list()
{
	string * names = new string[files.size()];
	int index = 0;

	for(const auto & f: files)
	{
		names[index++] = f.get_name();
	}

	return names;
}

File Directory::get_File(string name)
{

	unordered_set<File, file_hasher, file_comparator>::iterator found = files.find(name);
	return *found;


	// note: possibly delete below, from previous implementation

	/*
	// found is an iterator, so if it is dereferenced, the address of the element can be
	// obtained by using the & operator
	//
	// the goal is to return a pointer to the actual File object in the unordered_set, so
	// it can be modified
	File * file = &(*found);
	*/
}	
// destructor
Directory::~Directory()
{
	files.clear();
}

// mutators
void Directory::add_file(File f)
{
	files.insert(f);
}
void Directory::remove_file(string f)
{
	files.erase(f);
}
void Directory::remove_file(File f)
{
	files.erase(f.get_name());
}
