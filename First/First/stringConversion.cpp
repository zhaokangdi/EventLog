#include "stringConversion.h"
#include <string>

std::string stringConversion::ConvertLPCWSTRToStr(LPCWSTR pwszInput)
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

std::wstring stringConversion::ConvertLPSTRToWstring(const char* pszInput)
{
	int iLength = MultiByteToWideChar(CP_ACP, 0, pszInput, -1, NULL, 0);
	WCHAR* pszBuf = new WCHAR[iLength + 1];
	ZeroMemory(pszBuf, (iLength + 1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, 0, pszInput, -1, pszBuf, iLength);
	std::wstring wsOutput(pszBuf);
	delete[] pszBuf;
	return wsOutput;
}
