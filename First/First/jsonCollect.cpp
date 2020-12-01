#include "jsonCollect.h"
#include "evnetLog.h"
#include "stringConversion.h"

Json::Value jsonCollect::getJson(LPWSTR pwszPath)
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

std::string jsonCollect::getAllTypeJson()
{
	stringConversion stringConvert;

	Json::Value root;
	Json::Value Application = getJson(L"Application");
	Json::Value Security = getJson(L"Security");
	Json::Value Setup = getJson(L"Setup");
	Json::Value System = getJson(L"System");

	root["Application"] = Json::Value(Application);
	root["Security"] = Json::Value(Security);
	root["Setup"] = Json::Value(Setup);
	root["System"] = Json::Value(System);

	std::string sResult = stringConvert.json_to_string(root);
	return sResult;
}