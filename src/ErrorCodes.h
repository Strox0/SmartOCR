#pragma once

#define ERROR_SUCCESS 0
#define ERROR_COULD_NOT_OPEN_FILE 1
#define ERROR_INVALID_FILE_FORMAT 2
#define ERROR_FILE_DOES_NOT_EXIST 3
#define ERROR_COULD_NOT_LOAD_RESOURCE 4

const char* FormatError(unsigned short error_code);