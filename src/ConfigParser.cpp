#include "ConfigParser.h"
#include <filesystem>
#include <algorithm>

Parser::Parser(const char* file_path)
{
	if (!std::filesystem::exists(file_path))
	{
		m_error_code = ERROR_FILE_DOES_NOT_EXIST;
		return;
	}
	
	if (std::filesystem::path(file_path).extension() != ".titr")
	{
		m_error_code = ERROR_INVALID_FILE_FORMAT;
		return;
	}

	file.open(file_path, std::ios::in);
	if (!file.is_open())
	{
		m_error_code = ERROR_COULD_NOT_OPEN_FILE;
		return;
	}
}

// <name>:<regex_group> = <regex>

bool isnotnum(char c)
{
	return (c <= '9' && c >= '0') ? false : true;
}

void Parser::Parse()
{
	std::string line;
	while (getline(file, line)) 
	{
		std::istringstream iss(line);
		std::string groupname;
		std::string regex_group;
		std::string regex_string;

		if (!std::getline(iss, groupname, ':'))
			continue; 

		if (!std::getline(iss, regex_group, '='))
			continue; 

		if (!std::getline(iss, regex_string))
			continue; 

		std::remove(groupname.begin(), groupname.end(), ' ');

		std::remove_if(regex_group.begin(), regex_group.end(), isnotnum);

		regex_string = regex_string.substr(regex_string.find_first_not_of(' '));		

		if (groupname.empty() || regex_group.empty() || regex_string.empty())
			continue;

		config[regex_string];
		config.at(regex_string).regex_groups.push_back(regex_group);
		config.at(regex_string).names.push_back(groupname);
	}
}

const char* Parser::FormatError(short error_code) const
{
	switch (error_code)
	{
		case ERROR_SUCCESS:
			return "Success";
		case ERROR_COULD_NOT_OPEN_FILE:
			return "Could not open file";
		case ERROR_INVALID_FILE_FORMAT:
			return "Invalid file format";
		case ERROR_FILE_DOES_NOT_EXIST:
			return "File does not exist";
	default:
		return "Unknown error";
	}
}

