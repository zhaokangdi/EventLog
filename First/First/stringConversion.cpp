#include "stringConversion.h"
#include <string>
#include "value.h"
#include "writer.h"

std::string stringConversion::ConvertLPWSTRToStr(LPWSTR pwszInput)
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

std::wstring stringConversion::ConvertLPCWSTRToWStr(const char* pszInput)
{
	int iLength = MultiByteToWideChar(CP_ACP, 0, pszInput, -1, NULL, 0);
	WCHAR* pszBuf = new WCHAR[iLength + 1];
	ZeroMemory(pszBuf, (iLength + 1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, 0, pszInput, -1, pszBuf, iLength);
	std::wstring wsOutput(pszBuf);
	delete[] pszBuf;
	return wsOutput;
}

std::string stringConversion::WStringToString(std::wstring wsInput)
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

std::string stringConversion::json_to_string(const Json::Value &value)
{
	Json::StyledWriter writer;
	std::string rewrite = writer.write(value);
	return rewrite;
}
