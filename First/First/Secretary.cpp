#include "Secretary.h"

void Secretary::attach(Observer *pObserver)
{
	pListObservers.push_back(pObserver);
}

void Secretary::detach(Observer *pObserver)
{
	std::list<Observer *>::iterator iter = pListObservers.begin();
	while (iter != pListObservers.end())
	{
		if ((*iter) == pObserver)
		{
			pListObservers.erase(iter);
		}
		++iter;
	}
}

void Secretary::notify()
{
	std::list<Observer *>::iterator iter = pListObservers.begin();
	while (iter != pListObservers.end())
	{
		(*iter)->dealJson();
		++iter;
	}
}