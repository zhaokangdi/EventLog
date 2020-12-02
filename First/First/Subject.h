#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include <list>
#include <string>

class Observer;

class Subject
{
protected:
	std::list<Observer *> pListObservers;

public:
	std::string sJsonInfo;
	virtual void attach(Observer *) = 0;
	virtual void notify() = 0;
};

#endif