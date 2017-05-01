/**********************************************************************
 * James Stahl - Team Coyote
 **********************************************************************

 * File: Directory.h
 * Defines a directory system
 *
 * Implementation: files stored in unsorted_list, as the order does not matter.
 * Hash function implemented for constant-time access to any file.
 */

#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <iostream>
#include <unordered_set>
#include "File.h"
using namespace std;

/* hash functionality */
struct file_hasher
{
	// overload operator (), so this can be called as a function
	size_t operator()(const File & f) const
	{
		// returns an unsigned int using stl-provided hash function
		return hash<string>()(f.get_name());
	}
};

class Directory
{
	public:
		// constructors
		Directory();

		// destructor
		~Directory();

		// accessors/display functions
		void display_list();
		unordered_set<File, file_hasher> get_file_list();
		string * get_file_name_list();

		// mutators
		void add_file(File);
		void remove_file(string);
		void remove_file(File);
		//TODO: error handling for files that don't exist yet for remove, those that already do for add?

	private:
		unordered_set<File, file_hasher> files;
};
#endif
