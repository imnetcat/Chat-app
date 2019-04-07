#pragma once




VOID addConnection(fd_set master, int fdmax, int n) {
	struct sockaddr_storage remoteaddr; // адрес клиента
	socklen_t addrlen;
	char remoteIP[INET6_ADDRSTRLEN];

	cout << "[SERVER]--> accept" << endl;
	addrlen = sizeof remoteaddr;
	PROTO.clientSock.push_back(accept(PROTO.SOCK,
		(struct sockaddr *)&remoteaddr,
		&addrlen));
	if (PROTO.clientSock[n] == -1) {
		cout << "\r[SERVER]--> ERROR" << endl;
		closesocket(PROTO.clientSock[n]);
	}
	else {
		PROTO.connections++;
		FD_SET(PROTO.clientSock[n], &master); // добавить в главный массив
		if (PROTO.clientSock[n] > fdmax) { // отслеживаем максимальный номер
			fdmax = PROTO.clientSock[n];
		}
		cout << "[SERVER]--> New connection from " <<
			inet_ntop(remoteaddr.ss_family,
				get_in_addr((struct sockaddr*)&remoteaddr),
				remoteIP, INET6_ADDRSTRLEN)
			<< endl;
	}
}

DWORD WINAPI Server(CONST HANDLE sMutex) {
	WaitForSingleObject(sMutex, INFINITE);
	int nbytes;
	fd_set master; // главный список файловых дескрипторов
	FD_ZERO(&master); // очистка главного и временного массивов
	int fdmax; // максимальный номер файлового дескриптора
	SOCKET s;
	fd_set read_fds; // временный список файловых дескрипторов для select()
	FD_ZERO(&read_fds);
	const int max_client_buffer_size = 1024;
	SOCKET client_socket = INVALID_SOCKET;
	int j, n2;

	WSADATA wsaData; // служебная структура для хранение информации
	// о реализации Windows Sockets
	// старт использования библиотеки сокетов процессом
	// (подгружается Ws2_32.dll)
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Если произошла ошибка подгрузки библиотеки
	if (result != 0) {
		cout << "ОШИБКА при запуске WSAStartup" << endl;
		return result;
	}

	struct addrinfo* addr = NULL; // структура, хранящая информацию
	// об IP-адресе  слущающего сокета

	// Шаблон для инициализации структуры адреса
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET; 
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_protocol = IPPROTO_TCP; 
	hints.ai_flags = AI_PASSIVE; 

	// Инициализируем структуру, хранящую адрес сокета - addr
	PROTO.ADDR = getServerAddress();
	result = getaddrinfo(PROTO.ADDR, PROTO.PORT, &hints, &addr);
	if (result != 0) {
		cout << "ОШИБКА при получении данных о ip" << endl;
		WSACleanup(); 
		return 1;
	}

	// Создание сокета
	PROTO.SOCK = socket(addr->ai_family, addr->ai_socktype,
		addr->ai_protocol);
	if (PROTO.SOCK == INVALID_SOCKET) {
		cout << "ОШИБКА при создании сокета" << endl;
		freeaddrinfo(addr);
		WSACleanup();
		return 1;
	}

	// Привязываем сокет к IP-адресу
	result = bind(PROTO.SOCK, addr->ai_addr, (int)addr->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cout << "ОШИБКА при привязке сокета" << endl;
		freeaddrinfo(addr);
		closesocket(PROTO.SOCK);
		WSACleanup();
		return 1;
	}

	// Инициализируем слушающий сокет
	if (listen(PROTO.SOCK, SOMAXCONN) == SOCKET_ERROR) {
		cout << "ОШИБКА при попыке слушать сокет" << endl;
		closesocket(PROTO.SOCK);
		WSACleanup();
		return 1;
	}




	cout << "  Сервер открыт. Используеться Ethernet IP адрес : " << PROTO.ADDR << endl;




	FD_SET(PROTO.SOCK, &master);
	// сохранить наибольший файловый дескриптор
	fdmax = PROTO.SOCK; // вот он

	// главный цикл
	for (;;) {
		read_fds = master; // копируем
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			cout << "[SERVER]--> select " << endl;
		}
		// ищем готовые для чтения данные в существующих подключениях
		for (s = 0; s <= fdmax; s++) {
			if (FD_ISSET(s, &read_fds)) { // Есть!!
				for (j = 0; j < PROTO.clientSock.size(); j++) {
					if (PROTO.clientSock[j] == s) {
						break;
					}
				}
				if (s == PROTO.SOCK) {
					// обрабатываем новые подключения
					addConnection(master, fdmax, j);
				}
				else {
					// Обработка данных от клиента
					if ((nbytes = recv(s, buf, sizeof(buf), 0)) <= 0) {
						// Ошибка или соединение закрыто клиентом
						if (nbytes == 0) {
							// Соединение закрыто
							cout << "\r[SERVER]--> Соединение закрыто" << endl;
						}
						else {
							// Операция успешно завершена
							cout << "\r[SERVER]--> Операция успешно завершена" << endl;
						}
						// Закрываем сокет
						PROTO.connections--;
						closesocket(PROTO.clientSock[j]); // Пока!
						PROTO.clientSock.erase(PROTO.clientSock.end() - 1);
						FD_CLR(PROTO.clientSock[j], &master); // удалить из главного массива
					}
					else {
						// от клиента что-то получили

						std::string temp(buf);

						if (strlen(buf) == BUFSIZE + 1) {
							temp[BUFSIZE] = '\0';
						}

						ParseAndDecrypt(temp);

						ZeroMemory(&buf, sizeof(buf));
					}
				} // END обработка данных от клиента
			} // END есть новое входящее подключение
		} // END цикл по файловым дескрипторам
	}// END for(;;)

}
