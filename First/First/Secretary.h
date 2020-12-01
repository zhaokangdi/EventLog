#ifndef SECRETARY_H
#define SECRETARY_H

#include "Subject.h"
#include "Observer.h"
#include <iterator>

class Observer;

class Secretary :public Subject
{
public:
	void attach(Observer *pObserver);
	void detach(Observer *pObserver);
	void notify();
};

#endif