#include "stringConversion.h"
#include "Subject.h"
#include "Secretary.h"
#include "Observer.h"
#include "WriteFileObserver.h"
#include "ConsolePrintObserver.h"

#include <iostream>

void main()
{
	Subject *m_pSubject = new Secretary(); //创建观察者

	//被观察的对象
	WriteFileObserver *m_pWriteFile = new WriteFileObserver();
	ConsolePrintObserver *m_pConsolePrint = new ConsolePrintObserver();

	//加入观察队列
	m_pSubject->attach(m_pWriteFile);
	m_pSubject->attach(m_pConsolePrint);

	//Secretary m_secretaryTemp;
	//std::cout << "notify之前的时间" << m_secretaryTemp.getTime() << std::endl;
	//通知消息
	m_pSubject->notify();
	//std::cout << "notify之后的时间" << m_secretaryTemp.getTime() << std::endl;

	m_pWriteFile->dealJson();
	m_pConsolePrint->dealJson();

	system("pause");
}
