#include "Secretary.h"
#include "evnetLog.h"
#include "stringConversion.h"

/*
#include <time.h>
#include <stdio.h>
#include <iostream>
#pragma warning(disable:4996)
std::string Secretary::getTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[128];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "Now is %Y/%m/%d %H:%M:%S", timeinfo);
	return buffer;
}
*/

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
	pListObservers.push_back(pObserver);
}

void Secretary::notify()
{
	stringConversion m_stringConvert;

	Json::Value root;
	Json::Value Application;
	Json::Value Security;
	Json::Value Setup;
	Json::Value System;

	//std::cout << "得到信息之前的时间" << getTime() << std::endl;
	getJson(L"Application", Application); //36s+得到结果的时间
	getJson(L"Security", Security); //50s+得到结果的时间
	getJson(L"Setup", Setup);
	getJson(L"System", System); //16s+得到结果的时间
	//std::cout << "得到信息之后的时间" << getTime() << std::endl;

	//std::cout << "加入root之前的时间" << getTime() << std::endl;
	root["Application"] = Json::Value(Application); //7s，不可避免
	root["Security"] = Json::Value(Security); //15s
	root["Setup"] = Json::Value(Setup);
	root["System"] = Json::Value(System); //3s
	//std::cout << "加入root之后的时间" << getTime() << std::endl;

	//std::cout << "转string之前的时间" << getTime() << std::endl;
	std::string sResult = m_stringConvert.json_to_string(root);
	//std::cout << "转string之前的时间" << getTime() << std::endl;

	std::list<Observer *>::iterator iter = pListObservers.begin();
	while (iter != pListObservers.end())
	{
		(*iter)->setJson(sResult);
		++iter;
	}

	//std::cout << "函数结束后的时间" << getTime() << std::endl; //json处理53s
}