#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <windows.h>
#include <winevt.h>
#include <string>

class eventLog
{
public:
	void GetDifferentType(LPWSTR pwszPath);
	std::wstring GetProviderName(EVT_HANDLE hProviderMetadata, EVT_HANDLE hEvent);
	LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId);
};

#endif