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

	//std::cout << "�õ���Ϣ֮ǰ��ʱ��" << getTime() << std::endl;
	getJson(L"Application", Application); //36s+�õ������ʱ��
	getJson(L"Security", Security); //50s+�õ������ʱ��
	getJson(L"Setup", Setup);
	getJson(L"System", System); //16s+�õ������ʱ��
	//std::cout << "�õ���Ϣ֮���ʱ��" << getTime() << std::endl;

	//std::cout << "����root֮ǰ��ʱ��" << getTime() << std::endl;
	root["Application"] = Json::Value(Application); //7s�����ɱ���
	root["Security"] = Json::Value(Security); //15s
	root["Setup"] = Json::Value(Setup);
	root["System"] = Json::Value(System); //3s
	//std::cout << "����root֮���ʱ��" << getTime() << std::endl;

	//std::cout << "תstring֮ǰ��ʱ��" << getTime() << std::endl;
	std::string sResult = m_stringConvert.json_to_string(root);
	//std::cout << "תstring֮ǰ��ʱ��" << getTime() << std::endl;

	std::list<Observer *>::iterator iter = pListObservers.begin();
	while (iter != pListObservers.end())
	{
		(*iter)->setJson(sResult);
		++iter;
	}

	//std::cout << "�����������ʱ��" << getTime() << std::endl; //json����53s
}