#ifndef XMLPARSE_H
#define XMLPARSE_H

#include <windows.h>
#include <string>

class xmlParse
{
public:
	std::wstring GetXmlInfo(LPWSTR pwszMessage, std::string sInfoType);
};

#endif