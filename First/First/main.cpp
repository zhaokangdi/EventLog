#include "stringConversion.h"
#include "jsonCollect.h"
#include "Subject.h"
#include "Secretary.h"
#include "Observer.h"
#include "WriteFileObserver.h"
#include "ConsolePrintObserver.h"

void main()
{
	jsonCollect jsonCollection;
	Subject *pSubject = new Secretary(); //创建观察者

	//被观察的对象
	Observer *pWriteFile = new WriteFileObserver(pSubject);
	Observer *pConsolePrint = new ConsolePrintObserver(pSubject);

	//加入观察队列
	pSubject->attach(pWriteFile);
	pSubject->attach(pConsolePrint);

	pSubject->sJsonInfo = jsonCollection.getAllTypeJson();
	pSubject->notify();

	system("pause");
}
