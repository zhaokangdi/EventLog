#ifndef EVENTLOG_H
#define EVENTLOG_H

#include "value.h"
#include "writer.h"
#include <windows.h>
#include <winevt.h>
#include <string>
#include <vector>

class eventLog
{
public:
	std::string ConvertLPWSTRToStr(_In_ LPWSTR pwszInput);
	std::wstring ConvertLPCWSTRToWStr(_In_ const char* pszInput);
	std::string WStringToString(_In_ std::wstring wsInput);
	std::wstring GetXmlInfo(_In_ LPWSTR pwszMessage, _In_ std::string sInfoType);
	void GetDifferentType(_In_ LPWSTR pwszPath, _Out_ std::vector<Json::Value> &vecJsonInfo);
	LPWSTR GetMessageString(_In_ EVT_HANDLE hMetadata, _In_ EVT_HANDLE hEvent, _In_ EVT_FORMAT_MESSAGE_FLAGS FormatId);
};

#endif