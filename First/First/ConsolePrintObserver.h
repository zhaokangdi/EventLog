#ifndef CONSOLEPRINTOBSERVER_H
#define CONSOLEPRINTOBSERVER_H

#include "Observer.h"
#include <list>
#include <string>

class Subject;

class ConsolePrintObserver :public Observer
{
public:
	ConsolePrintObserver(Subject *pSub);
	void dealJson();
};

#endif