#ifndef WRITEFILEOBSERVER_H
#define WRITEFILEOBSERVER_H

#include "Observer.h"
#include <list>
#include <string>

class Subject;

class WriteFileObserver :public Observer
{
public:
	WriteFileObserver(Subject *pSub);
	void dealJson();
};

#endif