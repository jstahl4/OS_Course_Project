/**********************************************************************
 * Asif Rahman - Team Coyote
 **********************************************************************
 */

#pragma once
#include <string>
#include <map>
#include "atosAPI.h"

class UI
{
	enum class Commands { Unknown, Create, Delete, Dir, Edit, Type, Exit, TestIt };	// supported commands

public:
	UI();
	~UI();

	int run();

private:
	Commands					getCommand(std::string const& aLine) const;				// detetmine the command
	void						createFile(std::string const& aLine);				// processing the createFile command.
	void						deleteFile(std::string const& aLine);				// processing the deleteFile command.
	void						editFile(std::string const& aLine);				// processing the editFile command.
	void						typeFile(std::string const& aLine);				// processing the editFile command.
	void						dir() const;	// print the directory list.
	void						testIt() const;	// Test the ATOS-FS API commands
	std::vector<std::string>	parseLine(std::string const& aLine) const;				// parse the command, and create separate strings
	
private:
	std::map<Commands, std::string>	m_Commands;											// Assign commands id to the word of commmands.
	int								m_FileHandle;
};

