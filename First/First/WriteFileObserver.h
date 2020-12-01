#ifndef WRITEFILEOBSERVER_H
#define WRITEFILEOBSERVER_H

#include "Observer.h"
#include <string>

class WriteFileObserver :public Observer
{
public:
	void setJson(std::string sJsonInfoSet);
	std::string getJson();
	void dealJson();
};

#endif