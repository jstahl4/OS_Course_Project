/**********************************************************************
 * James Stahl - Team Coyote
 **********************************************************************
 *
 * File: Directory.cpp
 * Defines a directory system
 *
 * Implementation: files stored in unsorted_list, as the order does not matter.
 * Hash function implemented for constant-time access to any file.
 */

#include<iostream>
#include "Directory.h"

// constructors
Directory::Directory()
{

}

// accessors/display functions
void Directory::display_list() const
{
	for(const auto & f: files)
	{
		cout << f.get_name() << endl;
	}
}

unordered_set<File, file_hasher, file_comparator> Directory::get_file_list() const
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
	File * ret = new File();
    unordered_set<File, file_hasher, file_comparator>::iterator found = files.find(name);

    if(found == files.end())
    {
        cerr << "File named " << name << " not found. Returning default file object.";
    } else
    {
        *ret = *found;
    }

	return *ret;
}

File Directory::get_File(int index)
{
    File ret("");
    // iterate through all File objects and find the one with matching starting block
    for(const auto & file: files)
    {
        if(file.get_starting_block() == index)
        {
            ret = file;
            break;
        }
    }

    return ret;
}

File Directory::get_File(File f)
{
    File * ret = new File();
    unordered_set<File, file_hasher, file_comparator>::iterator found = files.find(f.get_name());

    if(found == files.end())
    {
        cerr << "File named " << f.get_name() << " not found. Returning default file object.";
    } else
    {
        *ret = *found;
    }

    return *ret;
}



// destructor
Directory::~Directory()
{
	files.clear();
}

// mutators
void Directory::add_file(File & f)
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
