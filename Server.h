#pragma once



SOCKET SendSock;

bool SendTo(string addr, string data) {
	
	data += '\0';
	int packet_Size = data.size();
	if (packet_Size > 256) {
		return false;
	}

	size_t pos = addr.find('.');
	string aStr = addr.substr(0, pos);
	addr.erase(0, pos + 1);
	pos = addr.find('.');
	string bStr = addr.substr(0, pos);
	addr.erase(0, pos + 1);
	pos = addr.find('.');
	string cStr = addr.substr(0, pos);
	addr.erase(0, pos + 1);
	pos = addr.find(':');
	string dStr = addr.substr(0, pos);
	string pStr = addr.substr(pos + 1, addr.length());

	unsigned int a = toIntA((char*)aStr.c_str());
	unsigned int b = toIntA((char*)bStr.c_str());
	unsigned int c = toIntA((char*)cStr.c_str());
	unsigned int d = toIntA((char*)dStr.c_str());
	unsigned short port = toIntA((char*)pStr.c_str());

	unsigned int destination_address = (a << 24) | (b << 16) | (c << 8) | d;
	unsigned short destination_port = port;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination_address);
	address.sin_port = htons(destination_port);

	const char *packet_data = data.c_str();
	int sent_bytes = sendto(SendSock, packet_data, packet_Size,
		0, (sockaddr*)&address, sizeof(sockaddr_in));

	if (sent_bytes != packet_Size)
	{
		return false;
	}
}

VOID WINAPI ServerHandle(LPVOID p) {
	THREADPARAMS params = *(THREADPARAMS *)p;
	PROTO.PORT = params.port;
	CONST HANDLE sMutex = params.mut;
	WaitForSingleObject(sMutex, INFINITE);

	PROTO.ADDR = getServerAddress();

	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);


	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == INVALID_SOCKET)
	{
		gui.processMessageEvent(conversation, "", "failed to create socket");
		return;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	inet_pton(AF_INET, PROTO.ADDR, &(address.sin_addr.s_addr));
	address.sin_port = htons(PROTO.PORT);

	if (bind(sock, (const sockaddr*)&address, sizeof(address)) == INVALID_SOCKET)
	{
		gui.processMessageEvent(conversation, "", "failed to bind socket");
		return;
	}

	SendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (SendSock <= 0)
	{
		printf("failed to create socket\n");
		return;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PROTO.PORT + 1);

	if (bind(SendSock, (const sockaddr*)&addr, sizeof(sockaddr_in)) < 0)
	{
		printf("failed to bind socket\n");
		return;
	}

	//  Сервер и клиент открыты, вывод ip сервера
	gui.WriteText(3, 24, PROTO.ADDR + string(":") + toStrA(PROTO.PORT), gui.inputID);



	while (true)
	{
		char packet_data[256];
		unsigned int maximum_packet_size = sizeof(packet_data);

		typedef int socklen_t;

		sockaddr_in from;
		socklen_t fromLength = sizeof(from);

		int received_bytes = recvfrom(sock, (char*)packet_data, maximum_packet_size,
			0, (sockaddr*)&from, &fromLength);

		if (received_bytes <= 0)
			break;

		unsigned int from_address = ntohl(from.sin_addr.s_addr);
		unsigned int from_port = ntohs(from.sin_port);

		// process received packet
		gui.processMessageEvent(conversation, "RECIVED: ", packet_data);
		cout << packet_data;
	}

	return;

}
