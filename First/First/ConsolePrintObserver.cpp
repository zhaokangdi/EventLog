#include "ConsolePrintObserver.h"
#include <iostream>
#include <Windows.h>
#include <string>

void ConsolePrintObserver::setJson(std::string &sJsonInfoSet)
{
	m_sJsonInfo = sJsonInfoSet;
}

void ConsolePrintObserver::dealJson()
{
	std::cout << "����̨��ӡ�۲����յ�֪ͨ�����ڿ���̨��ӡ��" << std::endl;

	int iLen = 0;
	int iLength = 0;
	wchar_t* pwszTemp = NULL;

	FILE *pOutFile = NULL; //�ļ�ָ��
	fopen_s(&pOutFile, "ConsolePrintObserver.txt", "w,ccs=UTF-8");
	if (NULL == pOutFile)
	{
		goto cleanup;
	}

	iLength = (int)m_sJsonInfo.length() + 1;
	iLen = MultiByteToWideChar(CP_ACP, 0, m_sJsonInfo.c_str(), iLength, 0, 0);
	pwszTemp = new wchar_t[iLen];
	if (NULL == pwszTemp)
	{
		goto cleanup;
	}

	MultiByteToWideChar(CP_ACP, 0, m_sJsonInfo.c_str(), iLength, pwszTemp, iLen);
	fwrite(pwszTemp, sizeof(wchar_t), wcslen(pwszTemp), pOutFile);
	
cleanup:
	if (pwszTemp != NULL)
	{
		delete[] pwszTemp;
		pwszTemp = NULL;
	}
	
	if (pOutFile != NULL)
	{
		fclose(pOutFile);
		pOutFile = NULL;
	}

	std::cout << "����̨��ӡ��ɡ�" << std::endl;
}