#include "Secretary.h"
#include "Observer.h"
#include "WriteFileObserver.h"
#include "ConsolePrintObserver.h"

#include <iostream>

void main()
{
	Secretary *pSecretary = new Secretary(); //创建观察者

	//被观察的对象
	WriteFileObserver *pWriteFile = new WriteFileObserver();
	ConsolePrintObserver *pConsolePrint = new ConsolePrintObserver();

	if ((pSecretary != NULL) && (pWriteFile != NULL) && (pConsolePrint != NULL))
	{
		//加入观察队列
		pSecretary->attach(pWriteFile);
		pSecretary->attach(pConsolePrint);

		//通知消息
		pSecretary->notify();

		pWriteFile->dealJson();
		pConsolePrint->dealJson();
	}
	
	if (pSecretary != NULL)
	{
		delete pSecretary;
		pSecretary = NULL;
	}

	if (pWriteFile != NULL)
	{
		delete pWriteFile;
		pWriteFile = NULL;
	}

	if (pConsolePrint != NULL)
	{
		delete pConsolePrint;
		pConsolePrint = NULL;
	}

	system("pause");
}
