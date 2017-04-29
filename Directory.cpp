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
		cout << f.get_name << endl;
	}
}

unsorted_set<File, file_hasher> Directory::get_file_list()
{
	return files;
}

string * Directory::get_file_name_list()
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
