#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#define ERROR_SUCCESS 0
#define ERROR_COULD_NOT_OPEN_FILE 1
#define ERROR_INVALID_FILE_FORMAT 2
#define ERROR_FILE_DOES_NOT_EXIST 3


struct _STM_info
{
	uint32_t reg_group;
	std::string regex_stm;
};

typedef std::unordered_map<std::string, _STM_info> ConfigRes;

class Parser
{
public:
	Parser(const char* file_path);
	
	void Parse();

	short Error() const { return m_error_code; }
	const char* FormatError(short error_code) const;

	const ConfigRes& GetResult() const { return m_config; }

private:
	std::fstream file;

	std::unordered_map<std::string, _STM_info> m_config;

	short m_error_code = 0;
};