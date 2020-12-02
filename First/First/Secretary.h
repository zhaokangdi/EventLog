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
	//std::string getTime();

	void getJson(LPWSTR pwszPath, Json::Value &jsonValue);
	void attach(Observer *pObserver);
	void notify();
};

#endif