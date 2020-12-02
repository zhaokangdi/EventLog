#ifndef CONSOLEPRINTOBSERVER_H
#define CONSOLEPRINTOBSERVER_H

#include "Observer.h"
#include <string>

class ConsolePrintObserver :public Observer
{
public:
	void setJson(std::string sJsonInfoSet);
	void dealJson();
};

#endif