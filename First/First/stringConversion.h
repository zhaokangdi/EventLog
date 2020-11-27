#ifndef STRINGCONVERSION_H
#define STRINGCONVERSION_H

#include <windows.h>
#include <string>

class stringConversion
{
public:
	std::string ConvertLPCWSTRToStr(LPCWSTR pwszInput);
	std::wstring ConvertLPSTRToWstring(const char* pszInput);
};

#endif