#include "tinyxml2.h"
#include "stringConversion.h"
#include "xmlParse.h"
#include <string>
#include <iostream>

std::wstring xmlParse::GetXmlInfo(LPWSTR pwszMessage, std::string sInfoType)
{
	stringConversion m_strConvert;

	tinyxml2::XMLElement* attributeApproachRoot = NULL;
	tinyxml2::XMLElement* attributeApproachElement = NULL;
	const char* pwszInfo = NULL;
	const char* pwszXml = NULL;
	std::wstring wsResult = L" ";

	//获取XML信息
	std::string sTemp = m_strConvert.ConvertLPWSTRToStr(pwszMessage);
	pwszXml = sTemp.data();
	tinyxml2::XMLDocument doc;
	doc.Parse(pwszXml);

	attributeApproachRoot = doc.FirstChildElement("Event");
	if (attributeApproachRoot)
	{
		attributeApproachElement = attributeApproachRoot->FirstChildElement("System");
		if (attributeApproachElement)
		{
			if ("PROVIDERNAME" == sInfoType)
			{
				tinyxml2::XMLElement* attributeProviderNameElement = attributeApproachElement->FirstChildElement("Provider");
				if (attributeProviderNameElement)
				{
					attributeProviderNameElement->QueryStringAttribute("Name", &pwszInfo);
					attributeProviderNameElement = NULL;
				}
			}
			else if ("COMPUTER" == sInfoType)
			{
				tinyxml2::XMLElement* attributeComputerElement = attributeApproachElement->FirstChildElement("Computer");
				if (attributeComputerElement)
				{
					pwszInfo = attributeComputerElement->GetText();
					attributeComputerElement = NULL;
				}
			}
			else if ("EVENTID" == sInfoType)
			{
				tinyxml2::XMLElement* attributeEventIDElement = attributeApproachElement->FirstChildElement("EventID");
				if (attributeEventIDElement)
				{
					pwszInfo = attributeEventIDElement->GetText();
					attributeEventIDElement = NULL;
				}
			}
			else if ("USERID" == sInfoType)
			{
				tinyxml2::XMLElement* attributeUserIDElement = attributeApproachElement->FirstChildElement("Security");
				if (attributeUserIDElement)
				{
					attributeUserIDElement->QueryStringAttribute("UserID", &pwszInfo);
					attributeUserIDElement = NULL;
				}
			}
			else if ("TIMECREATED" == sInfoType)
			{
				tinyxml2::XMLElement* attributeTimeCreatedElement = attributeApproachElement->FirstChildElement("TimeCreated");
				if (attributeTimeCreatedElement)
				{
					attributeTimeCreatedElement->QueryStringAttribute("SystemTime", &pwszInfo);
					attributeTimeCreatedElement = NULL;
				}
			}
			else
			{
				pwszInfo = "Wrong type of information required\n";
			}
		}
		else
		{
			std::cout << "Parse failure\n" << std::endl;
		}
	}
	else
	{
		std::cout << "Parse failure\n" << std::endl;
	}

	wsResult = m_strConvert.ConvertLPCWSTRToWStr(pwszInfo);

	if (attributeApproachElement)
	{
		attributeApproachElement = NULL;
	}

	if (attributeApproachRoot)
	{
		attributeApproachRoot = NULL;
	}

	pwszInfo = NULL;
	pwszXml = NULL;

	return wsResult;
}
