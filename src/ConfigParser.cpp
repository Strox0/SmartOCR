#include "ConfigParser.h"
#include <filesystem>
#include <algorithm>
#include <ranges>

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

bool Parser::DuplicateName(const std::string& name)
{
	for (const auto& i : m_config)
	{
		if (i.name == name)
			return true;
	}

	return false;
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

		if (DuplicateName(groupname))
			continue;

		m_config.push_back(_STM_info( groupname, std::stoi(regex_group), regex_string ));
	}
}