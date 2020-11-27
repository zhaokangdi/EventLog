#include "tinyxml2.h"
#include "stringConversion.h"
#include "xmlParse.h"
#include <string>

std::wstring xmlParse::GetXmlInfo(LPWSTR pwszMessage, std::string sInfoType)
{
	stringConversion strConvert;

	const char* pwszInfo = NULL;
	const char* pwszXml = NULL;
	std::wstring wsResult = L" ";

	//获取XML信息
	std::string sTemp = strConvert.ConvertLPCWSTRToStr(pwszMessage);
	pwszXml = sTemp.data();
	tinyxml2::XMLDocument doc;
	doc.Parse(pwszXml);

	tinyxml2::XMLElement* attributeApproachRoot = doc.FirstChildElement("Event")->FirstChildElement("System");

	if ("PROVIDERNAME" == sInfoType)
	{
		tinyxml2::XMLElement* attributeProviderNameElement = attributeApproachRoot->FirstChildElement("Provider");
		if (attributeProviderNameElement)
		{
			attributeProviderNameElement->QueryStringAttribute("Name", &pwszInfo);
		}
	}
	else if ("EVENTID" == sInfoType)
	{
		tinyxml2::XMLElement* attributeEventIDElement = attributeApproachRoot->FirstChildElement("EventID");
		if (attributeEventIDElement)
		{
			pwszInfo = attributeEventIDElement->GetText();
		}
	}
	else if ("USERID" == sInfoType)
	{
		tinyxml2::XMLElement* attributeUserIDElement = attributeApproachRoot->FirstChildElement("Security");
		if (attributeUserIDElement)
		{
			attributeUserIDElement->QueryStringAttribute("UserID", &pwszInfo);
		}
	}
	else if ("TIMECREATED" == sInfoType)
	{
		tinyxml2::XMLElement* attributeTimeCreatedElement = attributeApproachRoot->FirstChildElement("TimeCreated");
		if (attributeTimeCreatedElement)
		{
			attributeTimeCreatedElement->QueryStringAttribute("SystemTime", &pwszInfo);
		}
	}
	else
	{
		pwszInfo = "Wrong type of information required\n";
	}

	wsResult = strConvert.ConvertLPSTRToWstring(pwszInfo);
	return wsResult;
}
