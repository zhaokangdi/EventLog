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

		//Secretary m_secretaryTemp;
		//std::cout << "notify֮ǰ��ʱ��" << m_secretaryTemp.getTime() << std::endl;
		//֪ͨ��Ϣ
		pSecretary->notify();
		//std::cout << "notify֮���ʱ��" << m_secretaryTemp.getTime() << std::endl;

		pWriteFile->dealJson();
		pConsolePrint->dealJson();

		pWriteFile = NULL;
		pConsolePrint = NULL;
		pSecretary = NULL;
	}

	system("pause");
}
