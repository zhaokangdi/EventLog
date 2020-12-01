#ifndef JSONCOLLECT_H
#define JSONCOLLECT_H

#include "value.h"
#include "writer.h"
#include <windows.h>

class jsonCollect
{
public:
	Json::Value getJson(LPWSTR pwszPath);
	std::string getAllTypeJson();
};

#endif