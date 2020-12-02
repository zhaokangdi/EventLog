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
	void GetDifferentType(LPWSTR pwszPath, std::vector<Json::Value> &vecJsonInfo);
	void WriteFile(std::vector<std::string>* pvecJsonInfos, FILE* pOutFile);
	LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId);
};

#endif