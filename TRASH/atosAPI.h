/**********************************************************************
 * Team Coyote
 **********************************************************************
 */

#pragma once
#include <string>
#include <vector>

#define INVALID_ATOS_HANDLE_VALUE -1

namespace ATOS_API
{
	/* Create file 						*/
	// input: The filename					//
	// retun value: True if succeeded, otherwise false 	//
	bool	Create(std::string const& aFileName);

	/* Delete file 						*/
	// input: The filename					//
	// retun value: True if succeeded, otherwise false 	//
	bool	Delete(std::string const& aFileName);

	/* Open file 						*/
	// input: The filename					//
	// retun value: Handle of the file. if failed, return  	//
	// 		value is INVALID_ATOS_HANDLE_VALUE	//
	int	Open(std::string const& aFileName);

	/* Close file 						*/
	// input: The previously opened filehandle		//
	// retun value: true if succeeded, otherwise false 	//
	bool	Close(int aHandle);

	/* Read from file 					*/
	// input: The previously opened filehandle		//
	// retun value: number of characters actualy read	//
	int	Read(int /*handle*/, int /*numchards*/, char* /*buffer*/);

	/* Write to file 					*/
	// input: The previously opened filehandle		//
	// retun value: number of characters actualy written	//
	int	Write(int /*handle*/, int /*numchards*/, char const* /*buffer*/);

	/* Get the size of the file 				*/
	// input: The filename					//
	// retun value: The size of the file in blocks		//
	int	Stats(std::string const& aFileName);

	/* Get the size of the disk 				*/
	// input: none						//
	// retun value: The size of the disk in blocks		//
	int	TotalDiskSize();

	/* Collect the files 					*/
	// input: none						//
	// retun value: The list of files.			//
	std::vector<std::string> List();
}
