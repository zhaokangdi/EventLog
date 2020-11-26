#ifndef XMLPARSE_H
#define XMLPARSE_H

#include <windows.h>
#include <stdio.h>
#include <sddl.h>
#include <winevt.h>
#include <locale.h>
#include <string>

#include <exception>
#pragma comment(lib, "wevtapi.lib")

class xmlParse
{
public:
	LPSTR xmlParse::ConvertLPWSTRToLPSTR(LPWSTR pwszInput);
	LPWSTR xmlParse::ConvertLPSTRToLPWSTR(const char* pszInput);
	LPWSTR xmlParse::GetXmlInfo(LPWSTR pwszMessage, std::string sInfoType);
};

#endif