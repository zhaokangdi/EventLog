#include "evnetLog.h"
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

void eventLog::GetDifferentType(LPWSTR pwszPath, std::vector<Json::Value> &vecJsonInfo)
{
	EVT_HANDLE hProviderMetadata = NULL;
	EVT_HANDLE hResults = NULL;
	EVT_HANDLE hEvent = NULL;
	DWORD dwStatus = ERROR_SUCCESS;
	DWORD dwReturned = 0;

	if (NULL == pwszPath)
	{
		goto cleanup;
	}

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

	while (true)
	{
		BOOL bEvtNextSignal = EvtNext(hResults, 1, &hEvent, INFINITE, 0, &dwReturned);

		if (!bEvtNextSignal)
		{
			if (hEvent != NULL)
			{
				EvtClose(hEvent);
				hEvent = NULL;
			}

			break;
		}

		Json::Value jsonValue;

		LPWSTR pwszMessage = NULL;
		LPWSTR pwszXmlMessage = NULL;
		LPWSTR pwszProviderName = NULL;
		std::wstring wsProvider;

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* attributeApproachRoot = NULL;
		tinyxml2::XMLElement* attributeApproachElement = NULL;
		const char* pwszXml = NULL;

		PSID pSid = NULL;
		DWORD dwSize = MAX_NAME;
		SID_NAME_USE SidType = SidTypeUser;
		char szUserName[MAX_NAME] = { 0 };
		char szDomain[MAX_NAME] = { 0 };
		std::string sUserName;
		std::string sTemp;

		//XML
		pwszXmlMessage = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageXml);
		if (NULL == pwszXmlMessage)
		{
			goto whileCleanup;
		}

		sTemp = ConvertLPWSTRToStr(pwszXmlMessage);
		pwszXml = sTemp.data();

		if (pwszXml != NULL)
		{
			doc.Parse(pwszXml);
			attributeApproachRoot = doc.FirstChildElement("Event");

			if (attributeApproachRoot != NULL)
			{
				attributeApproachElement = attributeApproachRoot->FirstChildElement("System");

				if (attributeApproachElement != NULL)
				{
					//ProviderName
					wsProvider = GetXmlStringAttribute(attributeApproachElement, "Provider", "Name");
					const wchar_t* pwszTemp = wsProvider.c_str();
					pwszProviderName = (wchar_t *)(pwszTemp);

					//计算机（Computer）
					std::wstring wsComputer = GetXmlText(attributeApproachElement, "Computer");
					jsonValue["Computer"] = WStringToString(wsComputer.c_str());

					//事件ID（EventID）
					std::wstring wsEventID = GetXmlText(attributeApproachElement, "EventID");
					jsonValue["EventID"] = WStringToString(wsEventID.c_str());

					//来源（Provider）
					jsonValue["ProviderName"] = WStringToString(wsProvider.c_str());

					//用户（UserID）
					std::wstring wsUserID = GetXmlStringAttribute(attributeApproachElement, "Security", "UserID");
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
					std::wstring wsTimeCreated = GetXmlStringAttribute(attributeApproachElement, "TimeCreated", "SystemTime");
					jsonValue["TimeCreated"] = WStringToString(wsTimeCreated.c_str());

					free(pwszXmlMessage);
					pwszXmlMessage = NULL;
				}
			}
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

		if (hProviderNameMetadata)
		{
			EvtClose(hProviderNameMetadata);
			hProviderNameMetadata = NULL;
		}
		vecJsonInfo.push_back(jsonValue);

	whileCleanup:
		if (hEvent != NULL)
		{
			EvtClose(hEvent);
			hEvent = NULL;
		}
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

std::string eventLog::ConvertLPWSTRToStr(LPWSTR pwszInput)
{
	std::string sOutput;
	DWORD dwMinSize = 0;
	LPSTR pszStr = NULL;

	if (pwszInput != NULL)
	{
		dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, pwszInput, -1, NULL, 0, NULL, FALSE);
		if (0 == dwMinSize)
		{
			sOutput = "Conversion failure\n";
		}
		else
		{
			pszStr = new char[dwMinSize];
			WideCharToMultiByte(CP_OEMCP, NULL, pwszInput, -1, pszStr, dwMinSize, NULL, FALSE);
			if (NULL == pszStr)
			{
				goto cleanup;
			}

			sOutput = pszStr;
		}
	}

cleanup:
	if (pszStr != NULL)
	{
		delete[] pszStr;
		pszStr = NULL;
	}

	return sOutput;
}

std::wstring eventLog::ConvertLPCWSTRToWStr(const char* pszInput)
{
	int iLength = 0;
	WCHAR* pszBuf = NULL;
	std::wstring wsOutput;

	if (pszInput != NULL)
	{
		iLength = MultiByteToWideChar(CP_ACP, 0, pszInput, -1, NULL, 0);
		pszBuf = new WCHAR[iLength + 1];
		ZeroMemory(pszBuf, (iLength + 1) * sizeof(WCHAR));

		if (NULL == pszBuf)
		{
			goto cleanup;
		}

		MultiByteToWideChar(CP_ACP, 0, pszInput, -1, pszBuf, iLength);
		wsOutput = pszBuf;
	}

cleanup:
	if (pszBuf != NULL)
	{
		delete[] pszBuf;
		pszBuf = NULL;
	}

	return wsOutput;
}

std::string eventLog::WStringToString(std::wstring wsInput)
{
	std::string sOutput;
	int iLen = 0;
	char* pszBuff = NULL;

	iLen = WideCharToMultiByte(CP_ACP, 0, wsInput.c_str(), wsInput.size(), NULL, 0, NULL, NULL);
	if (iLen > 0)
	{
		pszBuff = new char[iLen + 1];
		WideCharToMultiByte(CP_ACP, 0, wsInput.c_str(), wsInput.size(), pszBuff, iLen, NULL, NULL);
		pszBuff[iLen] = '\0';

		if (NULL == pszBuff)
		{
			goto cleanup;
		}

		sOutput.append(pszBuff);
	}

cleanup:
	if (pszBuff != NULL)
	{
		delete[] pszBuff;
		pszBuff = NULL;
	}
	
	return sOutput;
}

std::wstring eventLog::GetXmlStringAttribute(tinyxml2::XMLElement* pAttributeApproachRoot, const char* szXmlElement, const char* szAttribute)
{
	const char* pwszInfo = NULL;
	std::wstring wsResult;

	if ((pAttributeApproachRoot != NULL) && (szXmlElement != NULL) && (szAttribute != NULL))
	{
		tinyxml2::XMLElement* attributeElement = pAttributeApproachRoot->FirstChildElement(szXmlElement);
		if (attributeElement != NULL)
		{
			attributeElement->QueryStringAttribute(szAttribute, &pwszInfo);

			if (pwszInfo != NULL)
			{
				wsResult = ConvertLPCWSTRToWStr(pwszInfo);
			}
		}
	}

	return wsResult;
}

std::wstring eventLog::GetXmlText(tinyxml2::XMLElement* pAttributeApproachRoot, const char* szXmlElement)
{
	const char* pwszInfo = NULL;
	std::wstring wsResult;

	if ((pAttributeApproachRoot != NULL) && (szXmlElement != NULL))
	{
		tinyxml2::XMLElement* attributeElement = pAttributeApproachRoot->FirstChildElement(szXmlElement);
		if (attributeElement != NULL)
		{
			pwszInfo = attributeElement->GetText();

			if (pwszInfo != NULL)
			{
				wsResult = ConvertLPCWSTRToWStr(pwszInfo);
			}
		}
	}

	return wsResult;
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