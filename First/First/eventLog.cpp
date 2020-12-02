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

void eventLog::GetDifferentType(LPWSTR pwszPath, std::vector<Json::Value> &vecJsonInfo)
{
	stringConversion m_stringConvert;
	xmlParse m_xmlInfo;

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
		char cUserName[MAX_NAME];
		char cDomain[MAX_NAME];

		//XML
		pwszXmlMessage = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageXml);
		if (pwszXmlMessage)
		{
			//ProviderName
			wsProvider = m_xmlInfo.GetXmlInfo(pwszXmlMessage, "PROVIDERNAME");
		}
		const wchar_t* pwszTemp = wsProvider.c_str();
		pwszProviderName = (wchar_t *)(pwszTemp);

		if (pwszXmlMessage)
		{
			//计算机（Computer）
			std::wstring wsComputer = m_xmlInfo.GetXmlInfo(pwszXmlMessage, "COMPUTER");
			jsonValue["Computer"] = m_stringConvert.WStringToString(wsComputer.c_str());

			//事件ID（EventID）
			std::wstring wsEventID = m_xmlInfo.GetXmlInfo(pwszXmlMessage, "EVENTID");
			jsonValue["EventID"] = m_stringConvert.WStringToString(wsEventID.c_str());

			//来源（Provider）
			jsonValue["ProviderName"] = m_stringConvert.WStringToString(wsProvider.c_str());

			//用户（UserID）
			std::wstring wsUserID = m_xmlInfo.GetXmlInfo(pwszXmlMessage, "USERID");
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
			std::wstring wsTimeCreated = m_xmlInfo.GetXmlInfo(pwszXmlMessage, "TIMECREATED");
			jsonValue["TimeCreated"] = m_stringConvert.WStringToString(wsTimeCreated.c_str());

			free(pwszXmlMessage);
			pwszXmlMessage = NULL;
		}

		EVT_HANDLE hProviderNameMetadata = EvtOpenPublisherMetadata(NULL, pwszProviderName, NULL, 0, 0);
		//操作信息（Message）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageEvent);
		if (pwszMessage)
		{
			jsonValue["EventMessage"] = m_stringConvert.ConvertLPWSTRToStr(pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//级别（Level）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageLevel);
		if (pwszMessage)
		{
			jsonValue["Level"] = m_stringConvert.ConvertLPWSTRToStr(pwszMessage);;
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//操作代码（Opcode）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageOpcode);
		if (pwszMessage)
		{
			jsonValue["Opcode"] = m_stringConvert.ConvertLPWSTRToStr(pwszMessage);;
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//关键字（Keyword）
		pwszMessage = GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageKeyword);
		if (pwszMessage)
		{
			jsonValue["keyword"] = m_stringConvert.ConvertLPWSTRToStr(pwszMessage);;
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

		vecJsonInfo.push_back(jsonValue);
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