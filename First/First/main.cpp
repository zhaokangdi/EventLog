#include "xmlParse.h"
#include <iostream>

void main()
{
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;

	xmlParse xmlparse;
	LPWSTR pwszInput = L"<Event xmlns='http://schemas.microsoft.com/win/2004/08/events/event'><System><Provider Name='Microsoft-Windows-Servicing' Guid='{BD12F3B8-FC40-4A61-A307-B7A013A069C1}'/><EventID>3</EventID><Version>0</Version><Level>0</Level><Task>1</Task><Opcode>0</Opcode><Keywords>0x8000000000000000</Keywords><TimeCreated SystemTime='2018-05-24T06:46:05.570944300Z'/><EventRecordID>2</EventRecordID><Correlation/><Execution ProcessID='7924' ThreadID='6252'/><Channel>Setup</Channel><Computer>zx-176</Computer><Security UserID='S-1-5-18'/></System><UserData><CbsPackageChangeState xmlns='http://manifests.microsoft.com/win/2004/08/windows/setup_provider'><PackageIdentifier>KB4134661</PackageIdentifier><IntendedPackageState>5064</IntendedPackageState><IntendedPackageStateTextized>Staged</IntendedPackageStateTextized><ErrorCode>0x800f0816</ErrorCode><Client>WindowsUpdateAgent</Client></CbsPackageChangeState></UserData></Event>";
	LPWSTR pwszOutput = xmlparse.GetXmlInfo(pwszInput, "PROVIDERNAME");
	wprintf(pwszOutput);

	system("pause");
}