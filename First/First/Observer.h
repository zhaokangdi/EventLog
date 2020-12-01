#ifndef OBSERVER_H
#define OBSERVER_H

#include "Subject.h"

class Subject;

class Observer
{
protected:
	class Subject *pSub;

public:
	Observer(Subject *pSub);
	virtual void dealJson() = 0;
};

#endif