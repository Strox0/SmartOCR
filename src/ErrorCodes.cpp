#include "ErrorCodes.h"

const char* FormatError(unsigned short error_code)
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
	case ERROR_COULD_NOT_LOAD_RESOURCE:
		return "Could not load resource";
	default:
		return "Unknown error";
	}
}