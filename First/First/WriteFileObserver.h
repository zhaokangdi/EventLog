#ifndef WRITEFILEOBSERVER_H
#define WRITEFILEOBSERVER_H

#include "Observer.h"
#include <string>

class WriteFileObserver :public Observer
{
public:
	void setJson(_In_ std::string &sJsonInfoSet);
	void dealJson();
};

#endif