#include "evnetLog.h"
#include "tinyxml2.h"
#include <windows.h>
#include <iostream>
#include <winevt.h>
#include <stdio.h>
#include <atlstr.h>  
#include <sddl.h>
#include <string>
#include <vector>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "wevtapi.lib")

#define MAX_NAME 256

std::string eventLog::ConvertLPWSTRToStr(LPWSTR pwszInput)
{
	std::string sOutput;
	DWORD dwMinSize = 0;
	LPSTR pszStr = NULL;

	dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, pwszInput, -1, NULL, 0, NULL, FALSE);
	if (0 == dwMinSize)
	{
		sOutput = "Conversion failure\n";
	}

	pszStr = new char[dwMinSize];
	WideCharToMultiByte(CP_OEMCP, NULL, pwszInput, -1, pszStr, dwMinSize, NULL, FALSE);
	sOutput = pszStr;
	delete[] pszStr;

	return sOutput;
}

std::wstring eventLog::ConvertLPCWSTRToWStr(const char* pszInput)
{
	int iLength = MultiByteToWideChar(CP_ACP, 0, pszInput, -1, NULL, 0);
	WCHAR* pszBuf = new WCHAR[iLength + 1];
	ZeroMemory(pszBuf, (iLength + 1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, 0, pszInput, -1, pszBuf, iLength);
	std::wstring wsOutput(pszBuf);
	delete[] pszBuf;
	return wsOutput;
}

std::string eventLog::WStringToString(std::wstring wsInput)
{
	std::string sOutput;
	int iLen = WideCharToMultiByte(CP_ACP, 0, wsInput.c_str(), wsInput.size(), NULL, 0, NULL, NULL);
	if (iLen <= 0)return sOutput;
	char* pszBuff = new char[iLen + 1];
	if (pszBuff == NULL)return sOutput;
	WideCharToMultiByte(CP_ACP, 0, wsInput.c_str(), wsInput.size(), pszBuff, iLen, NULL, NULL);
	pszBuff[iLen] = '\0';
	sOutput.append(pszBuff);
	delete[] pszBuff;

	return sOutput;
}

std::wstring eventLog::GetXmlInfo(LPWSTR pwszMessage, std::string sInfoType)
{
	tinyxml2::XMLElement* attributeApproachRoot = NULL;
	tinyxml2::XMLElement* attributeApproachElement = NULL;
	const char* pwszInfo = NULL;
	const char* pwszXml = NULL;
	std::wstring wsResult = L" ";

	//获取XML信息
	std::string sTemp = ConvertLPWSTRToStr(pwszMessage);
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

	wsResult = ConvertLPCWSTRToWStr(pwszInfo);

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

void eventLog::GetDifferentType(LPWSTR pwszPath, std::vector<Json::Value> &vecJsonInfo)
{
	EVT_HANDLE hProviderMetadata = NULL;
	EVT_HANDLE hResults = NULL;
	EVT_HANDLE hEvent = NULL;
	DWORD dwStatus = ERROR_SUCCESS;
	DWORD dwReturned = 0;
	LPWSTR pwszMessage = NULL;

	//查询Channel事件句柄
	hResults = EvtQuery(NULL, pwszPath, NULL, EvtQueryChannelPath);
	if (NULL == hResults)
	{
		dwStatus = GetLastError();

		if (ERROR_EVT_CHANNEL_NOT_FOUND == dwStatus)
		{
			wprintf(L"Channel %s was not found.\n", pwszPath);
		}
		else
		{
			wprintf(L"EvtQuery failed with %lu.\n", dwStatus);
		}

		goto cleanup;
	}

	hProviderMetadata = EvtOpenPublisherMetadata(NULL, NULL, NULL, 0, 0);
	if (NULL == hProviderMetadata)
	{
		wprintf(L"EvtOpenPublisherMetadata failed with %d\n", GetLastError());
		goto cleanup;
	}

	while (BOOL bEvtNextSignal = EvtNext(hResults, 1, &hEvent, INFINITE, 0, &dwReturned))
	{
		Json::Value jsonValue;

		LPWSTR pwszXmlMessage = NULL;
		LPWSTR pwszProviderName = NULL;
		std::wstring wsProvider = L"";

		PSID pSid = NULL;
		DWORD dwSize = MAX_NAME;
		SID_NAME_USE SidType;
		std::string sUserName = "";
		char szUserName[MAX_NAME];
		char szDomain[MAX_NAME];

		//XML
		pwszXmlMessage = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageXml);
		if (pwszXmlMessage != NULL)
		{
			//ProviderName
			wsProvider = GetXmlInfo(pwszXmlMessage, "PROVIDERNAME");
			const wchar_t* pwszTemp = wsProvider.c_str();
			pwszProviderName = (wchar_t *)(pwszTemp);

			//计算机（Computer）
			std::wstring wsComputer = GetXmlInfo(pwszXmlMessage, "COMPUTER");
			jsonValue["Computer"] = WStringToString(wsComputer.c_str());

			//事件ID（EventID）
			std::wstring wsEventID = GetXmlInfo(pwszXmlMessage, "EVENTID");
			jsonValue["EventID"] = WStringToString(wsEventID.c_str());

			//来源（Provider）
			jsonValue["ProviderName"] = WStringToString(wsProvider.c_str());

			//用户（UserID）
			std::wstring wsUserID = GetXmlInfo(pwszXmlMessage, "USERID");
			if (wsUserID != L"")
			{
				ConvertStringSidToSidW(wsUserID.c_str(), &pSid);
				LookupAccountSid(NULL, pSid, szUserName, &dwSize, szDomain, &dwSize, &SidType);
				sUserName = szUserName;
				if (pSid)
				{
					pSid = NULL;
				}
			}
			jsonValue["UserID"] = sUserName;

			//记录时间（TimeCreated）
			std::wstring wsTimeCreated = GetXmlInfo(pwszXmlMessage, "TIMECREATED");
			jsonValue["TimeCreated"] = WStringToString(wsTimeCreated.c_str());

			free(pwszXmlMessage);
			pwszXmlMessage = NULL;
		}

		EVT_HANDLE hProviderNameMetadata = EvtOpenPublisherMetadata(NULL, pwszProviderName, NULL, 0, 0);
		//操作信息（Message）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageEvent);
		if (pwszMessage != NULL)
		{
			jsonValue["EventMessage"] = ConvertLPWSTRToStr(pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//级别（Level）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageLevel);
		if (pwszMessage != NULL)
		{
			jsonValue["Level"] = ConvertLPWSTRToStr(pwszMessage);;
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//操作代码（Opcode）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageOpcode);
		if (pwszMessage != NULL)
		{
			jsonValue["Opcode"] = ConvertLPWSTRToStr(pwszMessage);;
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//关键字（Keyword）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageKeyword);
		if (pwszMessage != NULL)
		{
			jsonValue["keyword"] = ConvertLPWSTRToStr(pwszMessage);;
			free(pwszMessage);
			pwszMessage = NULL;
		}

		if (hEvent)
		{
			EvtClose(hEvent);
			hEvent = NULL;
		}

		if (hProviderNameMetadata)
		{
			EvtClose(hProviderNameMetadata);
			hProviderNameMetadata = NULL;
		}

		vecJsonInfo.push_back(jsonValue);
	}

cleanup:
	if (hResults)
	{
		EvtClose(hResults);
		hResults = NULL;
	}

	if (hProviderMetadata)
	{
		EvtClose(hProviderMetadata);
		hProviderMetadata = NULL;
	}
}

//从事件中获取指定的消息字符串。如果事件不包含指定的消息，则函数返回NULL。
LPWSTR eventLog::GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId)
{
	DWORD dwBufferSize = 102400;
	DWORD dwBufferUsed = 0;
	DWORD dwStatus = 0;

	//分配缓冲区（dwBufferSize * sizeof(WCHAR)）
	LPWSTR pwszBuffer = (LPWSTR)malloc(dwBufferSize * sizeof(WCHAR));
	memset(pwszBuffer, 0, dwBufferSize * sizeof(WCHAR));

	if (!EvtFormatMessage(hMetadata, hEvent, 0, 0, NULL, FormatId, dwBufferSize, pwszBuffer, &dwBufferUsed))
	{
		dwStatus = GetLastError();
		if (ERROR_INSUFFICIENT_BUFFER == dwStatus)
		{
			if ((EvtFormatMessageKeyword == FormatId))
			{
				pwszBuffer[dwBufferSize - 1] = L'\0';
			}
			else
			{
				dwBufferSize = dwBufferUsed;
			}

			pwszBuffer = (LPWSTR)malloc(dwBufferSize * sizeof(WCHAR));
			if (pwszBuffer)
			{
				EvtFormatMessage(hMetadata, hEvent, 0, 0, NULL, FormatId, dwBufferSize, pwszBuffer, &dwBufferUsed);
				if ((EvtFormatMessageKeyword == FormatId))
				{
					pwszBuffer[dwBufferUsed - 1] = L'\0';
				}
			}
			else
			{
				wprintf(L"Malloc failure\n");
			}
		}
		else if (ERROR_EVT_MESSAGE_NOT_FOUND == dwStatus || ERROR_EVT_MESSAGE_ID_NOT_FOUND == dwStatus)
		{

		}
		else
		{

		}
	}

	return pwszBuffer;
}