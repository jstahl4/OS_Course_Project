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

directory::directory(unsorted_list<File, file_hasher> f)
{
	// make deep copy
	for(const auto & file: f)
	{
		files.insert(file);
	}
}

// accessors/display functions
void directory::display_list()
{
	for(const auto & f: files)
	{
		cout << f.get_name << endl;
	}
}

unsorted_list<File, file_hasher> directory::get_file_list()
{
	return files;
}

string * directory::get_file_name_list()
{
	string * names = new string[files.size()];
	int index = 0;

	for(const auto & f: files)
	{
		names[index++] = f;
	}

	return names;
}

// destructor
directory::~directory()
{
	delete files;
}

// mutators
void directory::add_file(File f)
{
	files.insert(f);
}
void directory::remove_file(string f)
{
	files.erase(f);
}
void directory::remove_file(File f)
{
	files.erase(f.get_name());
}
