#pragma once


BOOL Connection(CONST CHAR *address) {
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
		return FALSE;
	}

	gui.WriteText(3, 24, "Connection made sucessfully", gui.inputID);

	if (addConnection(Sock)) {
		return FALSE;
	}

	return TRUE;
}