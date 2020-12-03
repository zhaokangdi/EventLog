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
