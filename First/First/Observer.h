#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>

class Observer
{
public:
	virtual void setJson(_In_ std::string &sJsonInfoSet) = 0;
	virtual void dealJson() = 0;

protected:
	std::string m_sJsonInfo;
};

#endif