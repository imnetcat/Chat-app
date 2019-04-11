#pragma once



fd_set master; // 
int fdmax;

BOOL addConnection(SOCKET s) {

	if (s == -1) {
		gui.processMessageEvent(conversation, "", "addConnection() ERROR");
		closesocket(s);
		return FALSE;
	}
	else {
		PROTO.connections++;
		FD_SET(s, &master); // 
		if (s > fdmax) { // 
			fdmax = s;
		}
		gui.processMessageEvent(conversation, "", "Новое подключение принято");
		/* Вывод подключенного ip
		cout << "New connection from " <<
			inet_ntop(remoteaddr.ss_family,
				get_in_addr((struct sockaddr*)&remoteaddr),
				remoteIP, INET6_ADDRSTRLEN)
			<< endl;
		*/
		return TRUE;
	}
}

DWORD WINAPI ServerHandle(CONST HANDLE sMutex) {
	WaitForSingleObject(sMutex, INFINITE);

	int nbytes;
	FD_ZERO(&master); // 
	SOCKET s;
	fd_set read_fds; // 
	FD_ZERO(&read_fds);
	const int max_client_buffer_size = 1024;
	SOCKET client_socket = INVALID_SOCKET;
	int j, n2;

	FD_SET(PROTO.SOCK, &master);
	// 
	fdmax = PROTO.SOCK; //

	// 
	for (;;) {
		read_fds = master; // 
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			gui.processMessageEvent(conversation, "", "select() ERROR");
		}
		// 
		for (s = 0; s <= fdmax; s++) {
			if (FD_ISSET(s, &read_fds)) { // 
				for (j = 0; j < PROTO.clientSock.size(); j++) {
					if (PROTO.clientSock[j] == s) {
						break;
					}
				}
				if (s == PROTO.SOCK) {
					// 
					struct sockaddr_storage remoteaddr;
					socklen_t addrlen;
					gui.processMessageEvent(conversation, "", "Входящее подключение");
					addrlen = sizeof remoteaddr;
					PROTO.clientSock.push_back(accept(PROTO.SOCK,
						(struct sockaddr *)&remoteaddr,
						&addrlen));

					addConnection(PROTO.clientSock[PROTO.clientSock.size()-1]);
				}
				else {
					// 
					if ((nbytes = recv(s, buf, sizeof(buf), 0)) <= 0) {
						// 
						if (nbytes == 0) {
							// 
							gui.processMessageEvent(conversation, "", "Подключение потеряно");
						}
						else {
							// 
							gui.processMessageEvent(conversation, "", "Подключение закрыто");
						}
						//
						PROTO.connections--;
						closesocket(PROTO.clientSock[j]); // 
						PROTO.clientSock.erase(PROTO.clientSock.end() - 1);
						FD_CLR(PROTO.clientSock[j], &master); // 
					}
					else {
						//

						std::string temp(buf);

						if (strlen(buf) == BUFSIZE + 1) {
							temp[BUFSIZE] = '\0';
						}

						ParseAndDecrypt(temp);

						ZeroMemory(&buf, sizeof(buf));
					}
				} // END 
			} // END 
		} // END 
	}// END for(;;)
}

VOID Server(VOID) {
	

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0) {
		gui.processMessageEvent(conversation, "", "WSAStartup() ERROR");
		return;
	}

	struct addrinfo* addr = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET; 
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_protocol = IPPROTO_TCP; 
	hints.ai_flags = AI_PASSIVE; 

	PROTO.ADDR = getServerAddress();
	result = getaddrinfo(PROTO.ADDR, PROTO.PORT, &hints, &addr);
	if (result != 0) {
		gui.processMessageEvent(conversation, "", "getaddrinfo() ERROR");
		WSACleanup(); 
		return;
	}

	PROTO.SOCK = socket(addr->ai_family, addr->ai_socktype,
		addr->ai_protocol);
	if (PROTO.SOCK == INVALID_SOCKET) {
		gui.processMessageEvent(conversation, "", "Socket create ERROR");
		freeaddrinfo(addr);
		WSACleanup();
		return;
	}

	result = bind(PROTO.SOCK, addr->ai_addr, (int)addr->ai_addrlen);
	if (result == SOCKET_ERROR) {
		gui.processMessageEvent(conversation, "", "bind() ERROR");
		freeaddrinfo(addr);
		closesocket(PROTO.SOCK);
		WSACleanup();
		return;
	}

	if (listen(PROTO.SOCK, SOMAXCONN) == SOCKET_ERROR) {
		gui.processMessageEvent(conversation, "", "listen() ERROR");
		closesocket(PROTO.SOCK);
		WSACleanup();
		return;
	}



	//  Сервер открыт, вывод ip сервера
	gui.WriteText(3, 24, PROTO.ADDR, gui.inputID);


	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerHandle, CreateMutex(NULL, FALSE, NULL), 0, NULL);


}
