#include "stringConversion.h"
#include "Subject.h"
#include "Secretary.h"
#include "Observer.h"
#include "WriteFileObserver.h"
#include "ConsolePrintObserver.h"

void main()
{
	Subject *pSubject = new Secretary(); //�����۲���

	//���۲�Ķ���
	WriteFileObserver *pWriteFile = new WriteFileObserver();
	ConsolePrintObserver *pConsolePrint = new ConsolePrintObserver();

	//����۲����
	pSubject->attach(pWriteFile);
	pSubject->attach(pConsolePrint);

	pSubject->notify(); //��JSON��Ϣ֪ͨ�����۲����

	//���۲����ֱ�ִ��
	pWriteFile->dealJson();
	pConsolePrint->dealJson();

	system("pause");
}
