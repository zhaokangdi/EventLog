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
	Subject *pSubject = new Secretary(); //�����۲���

	//���۲�Ķ���
	Observer *pWriteFile = new WriteFileObserver(pSubject);
	Observer *pConsolePrint = new ConsolePrintObserver(pSubject);

	//����۲����
	pSubject->attach(pWriteFile);
	pSubject->attach(pConsolePrint);

	pSubject->sJsonInfo = jsonCollection.getAllTypeJson();
	pSubject->notify();

	system("pause");
}
