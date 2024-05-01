#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "ErrorCodes.h"


struct _STM_info
{
	std::string name;
	uint32_t reg_group;
	std::string regex_stm;
};

typedef std::vector<_STM_info> ConfigRes;

class Parser
{
public:
	Parser(const char* file_path);
	
	void Parse();

	short Error() const { return m_error_code; }

	const ConfigRes& GetResult() const { return m_config; }

private:

	bool DuplicateName(const std::string& name);

	std::fstream file;

	ConfigRes m_config;

	short m_error_code = 0;
};