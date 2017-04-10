/**********************************************************************
 * Asif Rahman - Team Coyote
 **********************************************************************/
#include "UI.h"
#include <iostream>
#include <sstream>
#include <iterator>
#include <cstring> 

// Construct UI class.
UI::UI()
	: m_FileHandle(INVALID_ATOS_HANDLE_VALUE) // invalid handle
{
	// Convert Commands words to enum
	m_Commands[Commands::Create] = "CREATE";
	m_Commands[Commands::Delete] = "DELETE";
	m_Commands[Commands::Dir] = "DIR";
	m_Commands[Commands::Edit] = "EDIT";
	m_Commands[Commands::Type] = "TYPE";
	m_Commands[Commands::Exit] = "EXIT";
}

// Destruct UI class.
UI::~UI()
{
}


int UI::run()
{
	for (;;)									// forever
	{
		std::cout << "$$";
		std::string line;
		std::cin.clear();						// Clear std::cin (CTRL+Z/CTRL+D)
		std::getline(std::cin, line);			// read line from cin

		Commands o = getCommand(line);			// parse command from the line and processing it.
		switch (o)
		{
		case Commands::Unknown:
			if (line.size() != 0)				// Empty line is ok.
				std::cout << "$$Unknown command" << std::endl;
			break;
		case Commands::Create:
			createFile(line);
			break;
		case Commands::Delete:
			deleteFile(line);
			break;
		case Commands::Dir:
			dir();
			break;
		case Commands::Edit:
			editFile(line);
			break;
		case Commands::Type:
			typeFile(line);
			break;
		case Commands::Exit:
			return 0;							// Exit, no error.
		default:
			return -1;							// Fatal error, exit with error.
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
	if (cmd.size() != 2)									// One parameter is required.
	{
		std::cout << "$$Invalid arguments." << std::endl;
		return;
	}

	if (ATOS_API::Create(cmd[1]))						// Call ATOS create file API
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
	if (cmd.size() != 2)									// One parameter is required.
	{
		std::cout << "$$Invalid arguments." << std::endl;
		return;
	}

	if (ATOS_API::Delete(cmd[1]))						// Call ATOS Delete file API
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
	if (cmd[0] != m_Commands[Commands::Edit])				// Command shall be create.
	{
		std::cout << "$$Invalid command." << std::endl;
		return;
	}
	if (cmd.size() != 2)									// One parameter is required.
	{
		std::cout << "$$Invalid arguments." << std::endl;
		return;
	}

	int handle = ATOS_API::Open(cmd[1]);					// open file
	if (handle == INVALID_ATOS_HANDLE_VALUE)
	{
		std::cout << "$$ failed to open " << cmd[0] << " file";	// failed to open file
		return;
	}

	std::string line;
	while (std::getline(std::cin, line))					// Read line while not CTRL+D
	{
		if (ATOS_API::Write(handle, line.size(), line.c_str()) != line.size())
		{
			std::cout << "Failed to wite data data to file." << std::endl;
			break;
		}
	}

	if (ATOS_API::Close(handle))						// Close file handle
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
	}
	if (cmd.size() != 2)									// One parameter is required.
	{
		std::cout << "$$Invalid arguments." << std::endl;
		return;
	}

	int handle = ATOS_API::Open(cmd[1]);					// open file
	if (handle == INVALID_ATOS_HANDLE_VALUE)
	{
		std::cout << "$$ failed to open " << cmd[0] << " file";	// failed to open file
		return;
	}

	char buff[255];
	while (ATOS_API::Read(handle, sizeof(buff), buff ) > 0)
	{
		std::cout << buff;
	}

	if (ATOS_API::Close(handle))					// Close file
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
	std::vector<std::string> l = ATOS_API::List();									// Get the directory list.
	std::cout << "\tATOS-FS Directory Listing." << std::endl;						// Directory content header
	std::cout << "\tFILENAME\t\t\tSIZE(blks)" << std::endl;
	for (auto const& iT : l)
	{
		std::cout << "\t" << iT << "\t\t\t" << ATOS_API::Stats(iT) << std::endl;	// Rows.
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
