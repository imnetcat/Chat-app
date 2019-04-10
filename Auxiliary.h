#pragma once

void getAllChars() {
	for (int n = 0; n < 256; n++) {
		cout << n << ": " << (char)n << endl;
	}
	/*
	:
	179: │
    180: ┤
    191: ┐
    192: └
    193: ┴
    194: ┬
    195: ├
    196: ─
    197: ┼
	217: ┘
    218: ┌

	*/
}
bool catchErrr(int retVal) {
	if (retVal == SOCKET_ERROR)
	{
		cout << "Error: " << WSAGetLastError() << endl;
		WSACleanup();
		return true;
	}
	else {
		return false;
	}
}
int toIntA(char * wstr) {
	char * pEnd;
	return strtol(wstr, &pEnd, 10);
}
char  *toStrA(int integer) {
	char * pEnd;
	pEnd = new char[10];
	_itoa_s(integer, pEnd, 10, 10);
	return pEnd;
}
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
PSTR getServerAddress() {
	PMIB_IPADDRTABLE pIPAddrTable;
	DWORD dwSize = 0;
	IN_ADDR IPAddr;
	ULONG  ret;
	pIPAddrTable = new MIB_IPADDRTABLE[sizeof(MIB_IPADDRTABLE)];
	ret = GetIpAddrTable(pIPAddrTable, &dwSize, 0);        // Инициализация pIPAddrTable
	ret = GetIpAddrTable(pIPAddrTable, &dwSize, 0);        // Заполнение pIPAddrTable актуальными данными
	IPAddr.S_un.S_addr = (u_long)pIPAddrTable->table[0].dwAddr;
	PSTR stringAddr = new CHAR[16];
	inet_ntop_2(AF_INET, &IPAddr, stringAddr, 16);
	delete[] pIPAddrTable;
	return stringAddr;
}

WORD GetDefColor() {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	return info.wAttributes;
}

CONST WORD DEFFAULTCOLOR = GetDefColor();

void getAllColors() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFFAULTCOLOR);
	for (int n = 0; n < 256; n++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
		cout << n << ": " << "ABCDEFGH" << endl;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFFAULTCOLOR);
}