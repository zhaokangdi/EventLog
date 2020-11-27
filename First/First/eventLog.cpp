#include "eventLog.h"
#include "xmlParse.h"
#include <windows.h>
#include <winevt.h>
#include <string>

#pragma comment(lib, "wevtapi.lib")

void eventLog::GetDifferentType(LPWSTR pwszPath)
{
	xmlParse xmlInfo;
	eventLog eventLogs;

	EVT_HANDLE hProviderMetadata = NULL;
	EVT_HANDLE hResults = NULL;
	EVT_HANDLE hEvent = NULL;
	DWORD dwStatus = ERROR_SUCCESS;
	DWORD dwReturned = 0;
	LPWSTR pwszMessage = NULL;
	std::wstring wsMessage = L"";
	LPWSTR pwszProviderName = NULL;

	//��ѯChannel�¼����
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
		LPWSTR pwszXmlMessage = NULL;
		std::wstring wsTemp = eventLogs.GetProviderName(hProviderMetadata, hEvent);
		const wchar_t* pwszTemp = wsTemp.c_str();
		pwszProviderName = (wchar_t *)(pwszTemp);

		wprintf(L"ProviderName string: %s\n", pwszProviderName);
		EVT_HANDLE hProviderNameMetadata = EvtOpenPublisherMetadata(NULL, pwszProviderName, NULL, 0, 0);

		//XML
		pwszXmlMessage = eventLogs.GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageXml);
		if (pwszXmlMessage)
		{
			//�¼�ID��EventID��
			wsMessage = xmlInfo.GetXmlInfo(pwszXmlMessage, "EVENTID");
			wprintf(L"EventID message string: %s\n", wsMessage.c_str());

			//�û���UserID��
			wsMessage = xmlInfo.GetXmlInfo(pwszXmlMessage, "USERID");
			wprintf(L"UserID message string: %s\n", wsMessage.c_str());

			//��¼ʱ�䣨TimeCreated��
			wsMessage = xmlInfo.GetXmlInfo(pwszXmlMessage, "TIMECREATED");
			wprintf(L"TimeCreated message string: %s\n", wsMessage.c_str());

			free(pwszXmlMessage);
			pwszXmlMessage = NULL;
		}

		//�¼���Event��
		pwszMessage = eventLogs.GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageEvent);
		if (pwszMessage)
		{
			wprintf(L"Event message string: %s\n", pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//��־���ƣ�Channel��
		pwszMessage = eventLogs.GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageChannel);
		if (pwszMessage)
		{
			wprintf(L"Channel message string: %s\n", pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//��Դ��Provider��
		pwszMessage = eventLogs.GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageProvider);
		if (pwszMessage)
		{
			wprintf(L"Provider message string: %s\n", pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//����Level��
		pwszMessage = eventLogs.GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageLevel);
		if (pwszMessage)
		{
			wprintf(L"Level message string: %s\n", pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//�������루Opcode��
		pwszMessage = eventLogs.GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageOpcode);
		if (pwszMessage)
		{
			wprintf(L"Opcode message string: %s\n", pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//�������Task��
		pwszMessage = eventLogs.GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageTask);
		if (pwszMessage)
		{
			wprintf(L"Task message string: %s\n", pwszMessage);
			free(pwszMessage);
			pwszMessage = NULL;
		}

		//�ؼ��֣�Keyword��
		pwszMessage = eventLogs.GetMessageString(hProviderNameMetadata, hEvent, EvtFormatMessageKeyword);
		if (pwszMessage)
		{
			LPWSTR pTemp = pwszMessage;

			wprintf(L"Keyword message string: %s", pTemp);

			while (*(pTemp += wcslen(pTemp) + 1))
				wprintf(L", %s", pTemp);

			wprintf(L"\n");
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
		wprintf(L"\n");
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

std::wstring eventLog::GetProviderName(EVT_HANDLE hProviderMetadata, EVT_HANDLE hEvent)
{
	xmlParse xmlInfo;

	std::wstring swProviderName; //�ṩ��������Provider��

	LPWSTR pwszMessage = NULL; //��ȡ��Ϣ��XML��

	pwszMessage = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageXml);
	if (pwszMessage)
	{
		swProviderName = xmlInfo.GetXmlInfo(pwszMessage, "PROVIDERNAME");
		free(pwszMessage);
		pwszMessage = NULL;
	}

	return swProviderName;
}

//���¼��л�ȡָ������Ϣ�ַ���������¼�������ָ������Ϣ����������NULL��
LPWSTR eventLog::GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId)
{
	//���仺������10240 * sizeof(WCHAR)��
	LPWSTR pwszBuffer = (LPWSTR)malloc(10240 * sizeof(WCHAR));
	memset(pwszBuffer, 0, 10240 * sizeof(WCHAR));

	DWORD dwBufferSize = 10240;
	DWORD dwBufferUsed = 0;
	DWORD dwStatus = 0;

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
				wprintf(L"malloc failed\n");
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