#include "stringConversion.h"
#include "xmlParse.h"
#include "eventLog.h"
#include <string>

void main()
{
	setlocale(LC_ALL, "chs");

	eventLog eventLogs;
	eventLogs.GetDifferentType(L"SetUp");

	system("pause");
}
