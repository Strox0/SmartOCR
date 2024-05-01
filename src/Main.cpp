#include "ErrorCodes.h"

#include "ConfigParser.h"

#include "OCR.h"
#include <iostream>

#pragma comment(lib, "archive.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libsharpyuv.lib")
#pragma comment(lib, "Crypt32.lib")

int main(int argc,char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: <program> <path to image or directory>" << std::endl;
		return 1;
	}

	Parser parser("./config.titr");

	bool use_parser = false;

	if (parser.Error())
	{
		if (parser.Error() == ERROR_FILE_DOES_NOT_EXIST)
		{
			std::cout << "No config file found (./config.titr)" << std::endl;
			use_parser = false;
		}
		else
		{
			std::cout << FormatError(parser.Error()) << std::endl;
			return 1;
		}
	}
	else
		use_parser = true;

	OCR ocr(argv[1]);

	if (ocr.GetError())
	{
		std::cout << FormatError(ocr.GetError()) << std::endl;
		return 1;
	}

	if (use_parser)
	{
		parser.Parse();
		ocr.SetConfig(parser.GetResult());
	}

	ocr.Process();

	if (ocr.GetError())
	{
		std::cout << FormatError(ocr.GetError()) << std::endl;
		return 1;
	}
	else
		std::cout << "Done!";

	return 0;
}