#ifndef WRITEFILEOBSERVER_H
#define WRITEFILEOBSERVER_H

#include "Observer.h"
#include <string>

class Subject;

class WriteFileObserver :public Observer
{
public:
	void setJson(std::string sJsonInfoSet);
	void dealJson();
};

#endif