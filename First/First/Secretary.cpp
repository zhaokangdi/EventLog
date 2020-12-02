#include "Secretary.h"
#include "evnetLog.h"

std::string Secretary::json_to_string(const Json::Value &value)
{
	Json::StyledWriter writer;
	std::string rewrite = writer.write(value);
	return rewrite;
}

void Secretary::getJson(LPWSTR pwszPath, Json::Value &jsonValue)
{
	eventLog m_eventLogs;

	std::vector<Json::Value> vecJsonInfos;
	m_eventLogs.GetDifferentType(pwszPath, vecJsonInfos);

	for (unsigned int i = vecJsonInfos.size(); i > 0; i--)
	{
		jsonValue.append(vecJsonInfos[i - 1]);
	}
}

void Secretary::attach(Observer *pObserver)
{
	m_pListObservers.push_back(pObserver);
}

void Secretary::detach(Observer *pObserver)
{
	m_pListObservers.remove(pObserver);
}

void Secretary::notify()
{
	Json::Value root;
	Json::Value Application;
	Json::Value Security;
	Json::Value Setup;
	Json::Value System;

	getJson(L"Application", Application); //36s+得到结果的时间
	getJson(L"Security", Security); //50s+得到结果的时间
	getJson(L"Setup", Setup);
	getJson(L"System", System); //16s+得到结果的时间

	root["Application"] = Json::Value(Application); //7s，不可避免
	root["Security"] = Json::Value(Security); //15s
	root["Setup"] = Json::Value(Setup);
	root["System"] = Json::Value(System); //3s

	std::string sResult = json_to_string(root);

	std::list<Observer *>::iterator iter = m_pListObservers.begin();
	while (iter != m_pListObservers.end())
	{
		(*iter)->setJson(sResult);
		++iter;
	}
}