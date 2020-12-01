#ifndef SECRETARY_H
#define SECRETARY_H

#include "Subject.h"
#include "Observer.h"
#include "value.h"
#include "writer.h"
#include <iterator>
#include <windows.h>

class Secretary :public Subject
{
public:
	Json::Value getJson(LPWSTR pwszPath);
	std::string getAllTypeJson();
	void attach(Observer *pObserver);
	void detach(Observer *pObserver);
	void notify();
};

#endif