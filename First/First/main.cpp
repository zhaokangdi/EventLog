#include "Secretary.h"
#include "Observer.h"
#include "WriteFileObserver.h"
#include "ConsolePrintObserver.h"

#include <iostream>

void main()
{
	Secretary *pSecretary = new Secretary(); //�����۲���

	//���۲�Ķ���
	WriteFileObserver *pWriteFile = new WriteFileObserver();
	ConsolePrintObserver *pConsolePrint = new ConsolePrintObserver();

	if ((pSecretary != NULL) && (pWriteFile != NULL) && (pConsolePrint != NULL))
	{
		//����۲����
		pSecretary->attach(pWriteFile);
		pSecretary->attach(pConsolePrint);

		//֪ͨ��Ϣ
		pSecretary->notify();

		pWriteFile->dealJson();
		pConsolePrint->dealJson();

		pWriteFile = NULL;
		pConsolePrint = NULL;
		pSecretary = NULL;
	}

	system("pause");
}
