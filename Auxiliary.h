#pragma once

bool catchErrr(int retVal) {
	if (retVal == SOCKET_ERROR)
	{
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
struct THREADPARAMS {
	HANDLE mut;
	int port;
};
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
	inet_ntop(AF_INET, &IPAddr, stringAddr, 16);
	delete[] pIPAddrTable;
	return stringAddr;
}
