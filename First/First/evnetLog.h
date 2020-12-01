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
	std::vector<Json::Value> GetDifferentType(LPWSTR pwszPath);
	void WriteFile(std::vector<std::string>* pvecJsonInfos, FILE* pOutFile);
	std::wstring GetProviderName(EVT_HANDLE hProviderMetadata, EVT_HANDLE hEvent);
	LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId);
};

#endif