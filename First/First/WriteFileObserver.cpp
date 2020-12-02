#include "WriteFileObserver.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <string>

#include "Secretary.h"

void WriteFileObserver::setJson(std::string sJsonInfoSet)
{
	sJsonInfo = sJsonInfoSet;
}

void WriteFileObserver::dealJson()
{
	std::cout << "文件打印观察者收到通知，正在文件打印。" << std::endl;

	FILE *pOutFile = NULL; //文件指针
	fopen_s(&pOutFile, "WriteFileObserver.txt", "w,ccs=UTF-8");

	std::string sWriteFileInfo = sJsonInfo;
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

	std::cout << "文件打印完成。" << std::endl;
}