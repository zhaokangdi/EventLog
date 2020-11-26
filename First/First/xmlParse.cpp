#include "tinyxml2.h"
#include "xmlParse.h"
#include <string>
#include <cstring>

LPSTR xmlParse::ConvertLPWSTRToLPSTR(LPWSTR pwszInput)
{
	LPSTR pszOutput = NULL;

	try
	{
		if (pwszInput != NULL)
		{
			int iInputStrLen = wcslen(pwszInput);
			int iOutputStrLen = WideCharToMultiByte(CP_ACP, 0, pwszInput, iInputStrLen, NULL, 0, 0, 0) + 2;
			pszOutput = new char[iOutputStrLen];

			if (pszOutput)
			{
				memset(pszOutput, 0x00, iOutputStrLen);
				WideCharToMultiByte(CP_ACP, 0, pwszInput, iInputStrLen, pszOutput, iOutputStrLen, 0, 0);
			}
		}
	}
	catch (std::exception e)
	{
		wprintf(L"Exception\n");
	}

	return pszOutput;
}

LPWSTR xmlParse::ConvertLPSTRToLPWSTR(const char* pszInput)
{
	int iInputLen = strlen(pszInput) + 1;
	int iOutputLen = MultiByteToWideChar(CP_ACP, 0, pszInput, iInputLen, NULL, 0);
	LPWSTR pwszOutput = new WCHAR[iInputLen];
	MultiByteToWideChar(CP_ACP, 0, pszInput, iInputLen, pwszOutput, iOutputLen);
	return pwszOutput;
}

LPWSTR xmlParse::GetXmlInfo(LPWSTR pwszMessage, std::string sInfoType)
{
	LPWSTR pwszResult = NULL;
	const char* pwszInfo = NULL;

	//获取XML信息
	static const char* pwszXml = ConvertLPWSTRToLPSTR(pwszMessage);
	tinyxml2::XMLDocument doc;
	doc.Parse(pwszXml);

	if ("PROVIDERNAME" == sInfoType)
	{
		tinyxml2::XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement()->FirstChildElement("Provider");
		attributeApproachElement->QueryStringAttribute("Name", &pwszInfo);
	}
	else if ("EVENTID" == sInfoType)
	{
		tinyxml2::XMLElement* pEventIDElement = doc.FirstChildElement("Event")->FirstChildElement("System")->FirstChildElement("EventID");
		pwszInfo = pEventIDElement->GetText();
	}
	else if ("USERID" == sInfoType)
	{
		tinyxml2::XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement()->FirstChildElement("Security");
		attributeApproachElement->QueryStringAttribute("UserID", &pwszInfo);
	}
	else if ("TIMECREATED" == sInfoType)
	{
		tinyxml2::XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement()->FirstChildElement("TimeCreated");
		attributeApproachElement->QueryStringAttribute("SystemTime", &pwszInfo);
	}
	else 
	{
		pwszInfo = "Wrong type of information required\n";
	}

	pwszResult = ConvertLPSTRToLPWSTR(pwszInfo);
	return pwszResult;
}
