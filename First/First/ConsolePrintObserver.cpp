#include "ConsolePrintObserver.h"
#include <iostream>
#include <Windows.h>

ConsolePrintObserver::ConsolePrintObserver(Subject *pSub) : Observer(pSub)
{
}

void ConsolePrintObserver::dealJson()
{
	std::cout << "����̨��ӡ�۲����յ�֪ͨ�����ڿ���̨��ӡ��" << std::endl;

	FILE *pOutFile = NULL; //�ļ�ָ��
	fopen_s(&pOutFile, "ConsolePrintObserver.txt", "w,ccs=UTF-8");

	std::string sWriteFileInfo = pSub->sJsonInfo;
	int iLen = 0;
	int iLength = (int)sWriteFileInfo.length() + 1;
	iLen = MultiByteToWideChar(CP_ACP, 0, sWriteFileInfo.c_str(), iLength, 0, 0);
	wchar_t* pwszTemp = new wchar_t[iLen];
	MultiByteToWideChar(CP_ACP, 0, sWriteFileInfo.c_str(), iLength, pwszTemp, iLen);

	fwrite(pwszTemp, sizeof(wchar_t), wcslen(pwszTemp), pOutFile);
	delete[] pwszTemp;

	if (pOutFile)
	{
		fclose(pOutFile);
		pOutFile = NULL;
	}

	std::cout << "����̨��ӡ��ɡ�" << std::endl;
}