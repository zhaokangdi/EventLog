#include "evnetLog.h"
#include "stringConversion.h"
#include "xmlParse.h"
#include "value.h"
#include "writer.h"
#include <windows.h>
#include <winevt.h>
#include <string>
#include <iostream>
#include <atlstr.h>  
#include <sddl.h>
#include <windows.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "wevtapi.lib")

#define MAX_NAME 256

std::vector<Json::Value> eventLog::GetDifferentType(LPWSTR pwszPath)
{
	stringConversion stringConvert;
	xmlParse xmlInfo;
	std::vector<Json::Value> vecJsonInfos;

	EVT_HANDLE hProviderMetadata = NULL;
	EVT_HANDLE hResults = NULL;
	EVT_HANDLE hEvent = NULL;
	DWORD dwStatus = ERROR_SUCCESS;
	DWORD dwReturned = 0;
	LPWSTR pwszMessage = NULL;
	LPWSTR pwszProviderName = NULL;

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

		PSID pSid = NULL;
		DWORD dwSize = MAX_NAME;
		SID_NAME_USE SidType;
		std::string sUserName = "";
		char cUserName[MAX_NAME];
		char cDomain[MAX_NAME];

		LPWSTR pwszXmlMessage = NULL;
		std::wstring wsTemp = GetProviderName(hProviderMetadata, hEvent);
		const wchar_t* pwszTemp = wsTemp.c_str();
		pwszProviderName = (wchar_t *)(pwszTemp);

		EVT_HANDLE hProviderNameMetadata = EvtOpenPublisherMetadata(NULL, pwszProviderName, NULL, 0, 0);

		//XML
		pwszXmlMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageXml);
		if (pwszXmlMessage)
		{
			//计算机（Computer）
			std::wstring wsComputer = xmlInfo.GetXmlInfo(pwszXmlMessage, "COMPUTER");
			jsonValue["Computer"] = stringConvert.WStringToString(wsComputer.c_str());

			//事件ID（EventID）
			std::wstring wsEventID = xmlInfo.GetXmlInfo(pwszXmlMessage, "EVENTID");
			jsonValue["EventID"] = stringConvert.WStringToString(wsEventID.c_str());

			//来源（Provider）
			std::wstring wsProvider = xmlInfo.GetXmlInfo(pwszXmlMessage, "PROVIDERNAME");
			jsonValue["ProviderName"] = stringConvert.WStringToString(wsProvider.c_str());

			//用户（UserID）
			std::wstring wsUserID = xmlInfo.GetXmlInfo(pwszXmlMessage, "USERID");
			if (wsUserID != L"")
			{
				ConvertStringSidToSidW(wsUserID.c_str(), &pSid);
				LookupAccountSid(NULL, pSid, cUserName, &dwSize, cDomain, &dwSize, &SidType);
				sUserName = cUserName;
				if (pSid)
				{
					pSid = NULL;
				}
			}
			jsonValue["UserID"] = sUserName;

			//记录时间（TimeCreated）
			std::wstring wsTimeCreated = xmlInfo.GetXmlInfo(pwszXmlMessage, "TIMECREATED");
			jsonValue["TimeCreated"] = stringConvert.WStringToString(wsTimeCreated.c_str());

			free(pwszXmlMessage);
			pwszXmlMessage = NULL;
		}

		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageEvent);
		if (pwszMessage)
		{
			jsonValue["EventMessage"] = stringConvert.ConvertLPWSTRToStr(pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//级别（Level）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageLevel);
		if (pwszMessage)
		{
			jsonValue["Level"] = stringConvert.ConvertLPWSTRToStr(pwszMessage);;
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//操作代码（Opcode）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageOpcode);
		if (pwszMessage)
		{
			jsonValue["Opcode"] = stringConvert.ConvertLPWSTRToStr(pwszMessage);;
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//关键字（Keyword）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageKeyword);
		if (pwszMessage)
		{
			jsonValue["keyword"] = stringConvert.ConvertLPWSTRToStr(pwszMessage);;
			free(pwszMessage);
			pwszMessage = NULL;
		}

		if (hEvent)
		{
			EvtClose(hEvent);
		}

		if (hProviderNameMetadata)
		{
			EvtClose(hProviderNameMetadata);
		}

		vecJsonInfos.push_back(jsonValue);
	}

cleanup:
	if (hResults)
	{
		EvtClose(hResults);
	}

	if (hProviderMetadata)
	{
		EvtClose(hProviderMetadata);
	}

	return vecJsonInfos;
}

std::wstring eventLog::GetProviderName(EVT_HANDLE hProviderMetadata, EVT_HANDLE hEvent)
{
	xmlParse xmlInfo;

	std::wstring swProviderName; //提供程序名（Provider）

	LPWSTR pwszMessage = NULL; //获取信息（XML）

	pwszMessage = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageXml);
	if (pwszMessage)
	{
		swProviderName = xmlInfo.GetXmlInfo(pwszMessage, "PROVIDERNAME");
		free(pwszMessage);
		pwszMessage = NULL;
	}

	return swProviderName;
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
			//wprintf(L"EvtFormatMessage failed with %u\n", dwStatus);
		}
	}

	return pwszBuffer;
}