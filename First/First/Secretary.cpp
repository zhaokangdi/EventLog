#include "Secretary.h"
#include "evnetLog.h"

void Secretary::getJson(LPWSTR pwszPath, Json::Value &jsonValue)
{
	if (pwszPath != NULL)
	{
		eventLog eventLogs;

		std::vector<Json::Value> vecJsonInfos;
		eventLogs.GetDifferentType(pwszPath, vecJsonInfos);

		for (unsigned int i = vecJsonInfos.size(); i > 0; i--)
		{
			jsonValue.append(vecJsonInfos[i - 1]);
		}
	}
}

void Secretary::attach(Observer *pObserver)
{
	if (pObserver != NULL)
	{
		m_pListObservers.push_back(pObserver);
	}
}

void Secretary::detach(Observer *pObserver)
{
	if (pObserver != NULL)
	{
		m_pListObservers.remove(pObserver);
	}
}

void Secretary::notify()
{
	Json::Value root;
	Json::Value Application;
	Json::Value Security;
	Json::Value Setup;
	Json::Value System;

	getJson(L"Application", Application);
	getJson(L"Security", Security);
	getJson(L"Setup", Setup);
	getJson(L"System", System);

	root["Application"] = Json::Value(Application);
	root["Security"] = Json::Value(Security);
	root["Setup"] = Json::Value(Setup);
	root["System"] = Json::Value(System);

	std::string sResult = json_to_string(root);

	std::list<Observer *>::iterator iter = m_pListObservers.begin();
	while (iter != m_pListObservers.end())
	{
		(*iter)->setJson(sResult);
		++iter;
	}
}

std::string Secretary::json_to_string(const Json::Value &value)
{
	Json::StyledWriter writer;
	std::string sRewrite = writer.write(value);
	return sRewrite;
}