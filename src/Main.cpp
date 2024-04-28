#include "ConfigParser.h"
#include <iostream>

#pragma comment(lib, "archive.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libsharpyuv.lib")
#pragma comment(lib, "Crypt32.lib")

int main()
{
	Parser parser("./config.titr");

	if (parser.Error())
	{
		std::cout << parser.FormatError(parser.Error()) << std::endl;
		return 1;
	}

	parser.Parse();

	const std::unordered_map<std::string, _STM_Info>& config = parser.GetResult();

	for (const auto& [regex_stm,stm_info] : config)
	{
		std::cout << "Regex Statement: " << regex_stm << std::endl;

		for (size_t i = 0; i < stm_info.names.size(); i++)
		{
			std::cout << "\tName: " << stm_info.names[i] << "\n\tGroup: " << stm_info.regex_groups[i] << std::endl;
		}
	}

	return 0;
}