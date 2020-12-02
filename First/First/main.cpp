#include "stringConversion.h"
#include "Subject.h"
#include "Secretary.h"
#include "Observer.h"
#include "WriteFileObserver.h"
#include "ConsolePrintObserver.h"

#include <iostream>

void main()
{
	Subject *m_pSubject = new Secretary(); //�����۲���

	//���۲�Ķ���
	WriteFileObserver *m_pWriteFile = new WriteFileObserver();
	ConsolePrintObserver *m_pConsolePrint = new ConsolePrintObserver();

	//����۲����
	m_pSubject->attach(m_pWriteFile);
	m_pSubject->attach(m_pConsolePrint);

	//Secretary m_secretaryTemp;
	//std::cout << "notify֮ǰ��ʱ��" << m_secretaryTemp.getTime() << std::endl;
	//֪ͨ��Ϣ
	m_pSubject->notify();
	//std::cout << "notify֮���ʱ��" << m_secretaryTemp.getTime() << std::endl;

	m_pWriteFile->dealJson();
	m_pConsolePrint->dealJson();

	system("pause");
}
