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

	const ConfigRes& config = parser.GetResult();

	for (auto& [name,stm_info] : config)
	{
		std::cout << "Column name : " << name << std::endl;
		std::cout << "\tRegex expression : " << stm_info.regex_stm << std::endl;
		std::cout << "\tGroup of the data : " << stm_info.reg_group << std::endl;
	}

	return 0;
}