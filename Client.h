#pragma once


INT WINAPI Connection(CHAR* address) {
	WSADATA wsaData;
	int retVal = 0;

	WSAStartup(MAKEWORD(2, 2), (LPWSADATA)&wsaData);

	SOCKET Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in serverInfo;


	serverInfo.sin_family = AF_INET;
	inet_pton(AF_INET, address, &(serverInfo.sin_addr.s_addr));
	serverInfo.sin_port = htons(toIntA((CHAR*)PROTO.PORT));

	retVal = connect(Sock, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
	if (catchErrr(retVal)) {
		return 1;
	}

	cout << "Connection made sucessfully" << endl;




	return 0;
}