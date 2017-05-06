/**********************************************************************
 * Asif Rahman - Team Coyote
 **********************************************************************/

#include "DISK_API.cpp"  
#include <iostream>
#include <sstream>
#include <iterator>
#include <cstring>
#include "UI.h"

// Construct UI class.
UI::UI()
//	: m_FileHandle(INVALID_ATOS_HANDLE_VALUE) // Initialize the file handle with invalid handle value
{
	// Convert Commands words to enum. Increase efficiency.
	m_Commands[Commands::Create] = "CREATE";
	m_Commands[Commands::Delete] = "DELETE";
	m_Commands[Commands::Dir] = "DIR";
	m_Commands[Commands::Edit] = "EDIT";
	m_Commands[Commands::Type] = "TYPE";
	m_Commands[Commands::Exit] = "EXIT";
	m_Commands[Commands::TestIt] = "TESTIT";
    disk = new Disk();
}

// Destruct UI class.
UI::~UI()
{
}


int UI::run(Disk * disk)
{
	for (;;)							// run forever - if not type EXIT
	{
		std::cout << "$$";
		std::string line;
		std::cin.clear();					// Clear std::cin (CTRL+Z/CTRL+D)
		std::getline(std::cin, line);				// read line from cin

		Commands o = getCommand(line);				// parse command from the line and processing it.
		switch (o)
		{
		case Commands::Unknown:
			if (line.size() != 0)				// Empty line is ok.
				std::cout << "$$Unknown command" << std::endl;
			break;
		case Commands::Create:
			createFile(line);				// Create file
			break;
		case Commands::Delete:					// Delete file
			deleteFile(line);
			break;
		case Commands::Dir:					// List the directory content
			dir();
			break;
		case Commands::Edit:					// Edit file
			editFile(line);
			break;
		case Commands::Type:					// Show the content of the selected file
			typeFile(line);
			break;
		case Commands::TestIt:					// Test all ATOS-FS API function
			testIt();
			break;
		case Commands::Exit:
			return 0;					// Exit, no error.
		default:
			return -1;					// Fatal error, exit with error.
		}

	}
}

UI::Commands UI::getCommand(std::string const& aLine) const
{
	for (auto const& iT : m_Commands)
	{
		// searching the operation
		if (iT.second.size() <= aLine.size())
		{
			if (aLine.substr(0, iT.second.size()) == iT.second )
			{
				// Command found
				return iT.first;
			}
		}
	}
	return Commands::Unknown;
}

void UI::createFile(std::string const& aLine)
{
	// parsing the command.
	std::vector<std::string> cmd = parseLine(aLine);
	if (cmd[0] != m_Commands[Commands::Create])				// Command shall be create.
	{
		std::cout << "$$Invalid command." << std::endl;
		return;
	}
	if (cmd.size() != 2)							// One parameter is required.
	{
		std::cout << "$$Invalid arguments." << std::endl;
		return;
	}

	if (disk->Create(cmd[1]))						// Call ATOS-FS create file API
	{
		// API call succeeded.
		std::cout << "$$" << cmd[1] << " file created." << std::endl;
	}
	else
	{
		// API call failed.
		std::cout << "$$Failed to create " << cmd[1] << " file." << std::endl;
	}
}

// processing the deleteFile command.
void UI::deleteFile(std::string const& aLine)
{
	// parsing the command.
	std::vector<std::string> cmd = parseLine(aLine);
	if (cmd[0] != m_Commands[Commands::Delete])				// Command shall be create.
	{
		std::cout << "$$Invalid command." << std::endl;
		return;
	}
	if (cmd.size() != 2)							// One parameter is required.
	{
		std::cout << "$$Invalid arguments." << std::endl;
		return;
	}

	if (disk->Delete(cmd[1]))						// Call ATOS-FS Delete file API
	{
		// API call succeeded.
		std::cout << "$$" << cmd[1] << " file deleted." << std::endl;
	}
	else
	{
		// API call failed.
		std::cout << "$$Failed to delete " << cmd[1] << " file." << std::endl;
	}
}

// processing the editFile command.
void UI::editFile(std::string const& aLine)
{
	// parsing the command.
	std::vector<std::string> cmd = parseLine(aLine);
	if (cmd[0] != m_Commands[Commands::Edit])				// Command shall be CREATE.
	{
		std::cout << "$$Invalid command." << std::endl;
		return;
	}
	if (cmd.size() != 2)							// One parameter is required.
	{
		std::cout << "$$Invalid arguments." << std::endl;
		return;
	}

	File file = disk->Open(cmd[1]);					// open file
	if (file.get_name() == DEFAULT_NAME)
	{
        // output to cerr already included in File::get_File() method
		return;
	}

	std::string line;
	while (std::getline(std::cin, line))					// Read line while not CTRL+D
	{
		char* buffer;
		string s = file.get_name();
		if (disk->Write(file, buffer) != line.size()+1) // +1 for ASCIIZ, test required!
		{
			std::cout << "$$Failed to wite data data to file." << std::endl;
			break;
		}
	}

	if (disk->Close(file.get_name()))						// Close file handle
	{
		// Successfully close
	}
	else
	{
		// failed to close files.
		std::cout << "$$Failed to close file.";
	}
	
}

// processing the editFile command.
void UI::typeFile(std::string const& aLine)
{
	// parsing the command.
	std::vector<std::string> cmd = parseLine(aLine);
	if (cmd[0] != m_Commands[Commands::Type])				// Command shall be create.
	{
		std::cout << "$$Invalid command." << std::endl;
		return;
	}
	if (cmd.size() != 2)									// One parameter is required.
	{
		std::cout << "$$Invalid arguments." << std::endl;
		return;
	}

	File file = disk->Open(cmd[1]);					// open file
	if (file.get_name() == DEFAULT_NAME)
	{
		std::cout << "$$failed to open " << cmd[0] << " file";	// failed to open file
		return;
	}

	string buff;
	buff = disk->Read(file.get_name());

    std::cout << buff;


	if (disk->Close(file.get_name()))					// Close file
	{
		// Successfully close
	}
	else
	{
		// failed to close files.
		std::cout << "$$Failed to close file.";
	}

}

// print the directory list.
void UI::dir() const
{
	uint64_t sum = 0;						// Total used space.
	std::vector<std::string> l = disk->List();									// Get the directory list from ATOS-FS.
	std::cout << "\tATOS-FS Directory Listing." << std::endl;						// Directory content header
	std::cout << "\tFILENAME\t\t\tSIZE(blks)" << std::endl;
	for (auto const& iT : l)
	{
		std::cout << "\t" << iT << "\t\t\t" << disk->Stats(iT) << std::endl;	// print rows.
		sum += disk->Stats(iT);							// Accumulate the size of the files.
	}
	std::cout << "\tFREE SPACE " << disk->numBlocks - sum << " blks" << std::endl;
}

// Test it all ATOS-FS function.
void UI::testIt() const
{
	if (!disk->Create("ASD1") )			// Try to create ASD1 file
	{
		std::cout << "$$Failed to create ASD1 file" << std::endl;
	}
		File handle = disk->Open("ASD1"); 		// Open ASD1 file
//	if (handle == INVALID_ATOS_HANDLE_VALUE) 	// Check the result of the Open ATOS-FS API call
//	{
//		std::cout << "$$Failed to open ASD1 file." << std::endl; // Failed to open ASD1 file
//	}

		int cnt = disk->Write(handle, "Something"); // Write "Something" in ASCIIZ to ASD1 file
		std::cout << "$$" << cnt << " bytes written. (-1 is error)" << std::endl;
		disk->Close(handle.get_name());		// Close the file handle

		handle = disk->Open("ASD1");	// Open ASD1 file (seek 0!!!)
//		if (handle == INVALID_ATOS_HANDLE_VALUE)// Check the result of the Open ATOS-FS API call
//		{
//			std::cout << "$$Failed to open ASD1 file" << std::endl; // Failed to open ASD1 file
//		}

			string buff;
			buff = disk->Read(handle.get_name()); // Read contents from ASD1 file

            std::cout << buff;

			disk->Close(handle.get_name()); 	// Close the file handle

		dir();					// print the directory structure.

	if (disk->Delete("ASD1"))			// Try to delete the ASD1 file
	{
		std::cout << "$$ASD1 file successfuly deleted." << std::endl;
	}
	else
	{
		std::cout << "$$Failed to delete ASD1 file." << std::endl;
	}
}

// parse the command, and create separate strings from each word.
std::vector<std::string> UI::parseLine(std::string const& aLine) const
{
	std::istringstream ss(aLine);
	std::istream_iterator<std::string> begin(ss), end;
	std::vector<std::string> res(begin, end);
	if (res.size() < 0) // Internal error, no command found
		std::cout << "$$Command not found." << std::endl;

	return res;
}
