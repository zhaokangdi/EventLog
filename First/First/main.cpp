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

		//Secretary m_secretaryTemp;
		//std::cout << "notify之前的时间" << m_secretaryTemp.getTime() << std::endl;
		//通知消息
		pSecretary->notify();
		//std::cout << "notify之后的时间" << m_secretaryTemp.getTime() << std::endl;

		pWriteFile->dealJson();
		pConsolePrint->dealJson();

		pWriteFile = NULL;
		pConsolePrint = NULL;
		pSecretary = NULL;
	}

	system("pause");
}
