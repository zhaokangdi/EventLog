#include "Secretary.h"
#include "evnetLog.h"
#include "stringConversion.h"

Json::Value Secretary::getJson(LPWSTR pwszPath)
{
	eventLog eventLogs;
	std::vector<Json::Value> vecJsonInfos = eventLogs.GetDifferentType(pwszPath);
	Json::Value jsonValue;

	for (unsigned int i = vecJsonInfos.size(); i > 0; i--)
	{
		jsonValue.append(vecJsonInfos[i - 1]);
	}

	return jsonValue;
}

std::string Secretary::getAllTypeJson()
{
	stringConversion stringConvert;

	Json::Value root;
	Json::Value Application = getJson(L"Application");
	//Json::Value Security = getJson(L"Security");
	//Json::Value Setup = getJson(L"Setup");
	//Json::Value System = getJson(L"System");

	root["Application"] = Json::Value(Application);
	//root["Security"] = Json::Value(Security);
	//root["Setup"] = Json::Value(Setup);
	//root["System"] = Json::Value(System);

	std::string sResult = stringConvert.json_to_string(root);
	return sResult;
}

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
		(*iter)->setJson(getAllTypeJson());
		++iter;
	}
}