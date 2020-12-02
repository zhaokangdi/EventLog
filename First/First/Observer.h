#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>

class Observer
{
protected:
	std::string sJsonInfo;

public:
	virtual void setJson(std::string sJsonInfoSet) = 0;
	virtual void dealJson() = 0;
};

#endif