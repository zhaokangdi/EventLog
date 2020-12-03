#ifndef SECRETARY_H
#define SECRETARY_H

#include "Observer.h"
#include "value.h"
#include "writer.h"
#include <iterator>
#include <windows.h>
#include <list>

class Secretary
{
public:
	void getJson(_In_ LPWSTR pwszPath, _Out_ Json::Value &jsonValue);
	void attach(_In_ Observer *pObserver);
	void detach(_In_ Observer *pObserver);
	void notify();

private:
	std::string json_to_string(_In_ const Json::Value &value);

private:
	std::list<Observer *> m_pListObservers;
};

#endif