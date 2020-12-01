#ifndef STRINGCONVERSION_H
#define STRINGCONVERSION_H

#include <windows.h>
#include <string>
#include "value.h"
#include "writer.h"

class stringConversion
{
public:
	std::string ConvertLPWSTRToStr(LPWSTR pwszInput);
	std::wstring ConvertLPCWSTRToWStr(const char* pszInput);
	std::string WStringToString(std::wstring wstr);
	std::string stringConversion::json_to_string(const Json::Value &value);
};

#endif