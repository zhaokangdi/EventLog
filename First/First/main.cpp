#include "stringConversion.h"
#include "Subject.h"
#include "Secretary.h"
#include "Observer.h"
#include "WriteFileObserver.h"
#include "ConsolePrintObserver.h"

void main()
{
	Subject *pSubject = new Secretary(); //创建观察者

	//被观察的对象
	WriteFileObserver *pWriteFile = new WriteFileObserver();
	ConsolePrintObserver *pConsolePrint = new ConsolePrintObserver();

	//加入观察队列
	pSubject->attach(pWriteFile);
	pSubject->attach(pConsolePrint);

	pSubject->notify(); //将JSON信息通知到被观察对象

	//被观察对象分别执行
	pWriteFile->dealJson();
	pConsolePrint->dealJson();

	system("pause");
}
