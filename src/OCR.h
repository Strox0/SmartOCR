#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "ConfigParser.h"

class OCR
{
public:

	OCR(const std::string& path);
	
	void SetConfig(const ConfigRes& config);

	void Process();

	uint16_t GetError() const { return m_error_code; }

private:

	void CreateOutput(std::vector<std::string>& results);

	ConfigRes m_config;
	bool m_config_set = false;
	std::string m_path;
	bool m_directory;

	uint16_t m_error_code;

};