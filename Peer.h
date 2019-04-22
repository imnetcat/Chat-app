#pragma once

SOCKET sock;
const char* host = "chatstunserver.herokuapp.com";
WORD port = 80;

BOOL SetUpConnection()
{
	struct hostent* hp;
	unsigned int addr;
	struct sockaddr_in server;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		return FALSE;

	if (inet_addr(host) == INADDR_NONE)
	{
		hp = gethostbyname(host);
	}
	else
	{
		addr = inet_addr(host);
		hp = gethostbyaddr((char*)& addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(sock);
		return FALSE;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if (connect(sock, (struct sockaddr*) & server, sizeof(server)))
	{
		closesocket(sock);
		return FALSE;
	}

	return TRUE;
}

BOOL SendHandshake() {
	stringstream buffer;

	buffer << "GET / HTTP/1.0" << "\r\n";
	buffer << "Host: " << host << "\r\n";
	buffer << "Connection: Upgrade" << "\r\n";
	buffer << "Pragma: no-cache" << "\r\n";
	buffer << "Cache-Control: no-cache" << "\r\n";
	buffer << "Upgrade: websocket" << "\r\n";
	buffer << "Sec-WebSocket-Version: 13" << "\r\n";
	buffer << "Accept-Encoding : gzip, deflate, br" << "\r\n";
	buffer << "Sec-WebSocket-Key: ya+IeOgWbUHw3k4e8FzeMA==" << "\r\n";
	buffer << "\r\n";
	int ret = send(sock, buffer.str().c_str(), buffer.str().length(), 0);
	if (ret == SOCKET_ERROR) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

BOOL SendTo(string data) {

	vector<BYTE> DATA; //

	long float fragments = 1;
	bool fragmentation = false;

	//  FIN = 1 (1 бит)
	//  RSV1 = 0   (1 бит)
	//  RSV2 = 0   (1 бит)
	//  RSV3 = 0   (1 бит)
	//  OPCODE = 0001 (4 бита)
	DATA.push_back(0x81); // set FIN + OPCODE (8 битов)
	//  MASK = 0 (1 бит)

	if (data.length() <= 124) {
		unsigned int data_length = data.length();
		DATA.push_back(data_length); // set LENGTH (7 битов)
	}
	else if (data.length() <= 65536) {
		unsigned int data_length = data.length();
		DATA.push_back(126);         // set LENGTH to extended (7 битов)
		DATA.push_back(data_length / 256); // set EXTLENGTH (8 битов)
		DATA.push_back(data_length % 256); // set EXTLENGTH (8 битов)
	}
	else {
		fragmentation = true;
		long float data_length = data.length();
		fragments = data_length / 65536;
	}

	//  MASK KEY = 0

	for (int n = 0; n < data.size(); n++) {
		DATA.push_back(data[n]); // set DATA
	}

	BYTE frame[65536];
	for (int n = 1; n <= fragments; n++) {
		if (fragments > 1 && n < fragments) {
			DATA.push_back(0x1); // set FIN = 0 && OPCODE = 1
		}

		int l;
		for (int k = 0; k < 65536; k++) {
			if (k == DATA.size()) {
				l = k;
				break;
			}
			frame[k] = DATA[n * 65536 - 65536 + k];
		}

		frame[l] = '\0';

		int ret = send(sock, (char*)frame, l, 0);
		if (ret == SOCKET_ERROR){
			return FALSE;
		}
	}

	return TRUE;
}

int SearchCharInVector(char a, vector <char> b) {
	int n;
	for (n = 0; n < b.size()-1; n++) {
		if (b[n] == a)
			break;
	}
	if (n == b.size())
	{
		return -1;
	} else {
		return n;
	}
}
vector <char> VectorSubstr(vector <char> v, int a, int b) {
	vector <char> ret;
	for (int n = a; n <= b; n++) {
		if (v[n] != -127) {
			ret.push_back(v[n]);
		}
	}
	return ret;
}

void WINAPI receiveData(CONST HANDLE sMutex){
	WaitForSingleObject(sMutex, INFINITY);
	int data_size;
	char buffer[512];
	string received_data;
	long int full_data_size = 0;
	int received_data_size = 0;
	while (true)
	{
		data_size = recv(sock, buffer, sizeof(buffer), 0);

		if (data_size <= 0) {
			break;
		}

		vector <char> data;
		for (int n = 0; n < data_size; n++) {
			data.push_back(buffer[n]);
		}

		int n = 0;
		int pos;
		while ((data.size() > 0) && ((pos = SearchCharInVector(-127, data)) != -1)) {
			n++;
			vector <char> frame = data;
			data.erase(data.begin(), data.begin()+pos+1);
			frame = VectorSubstr(frame, 0, pos);

			if (n == 1)
			{
				if (received_data == "")
					continue;

				received_data_size += frame.size();
				frame.push_back('\0');
				received_data += frame.data();

				if (received_data == "ping") 
				{
					continue;
				}

				if (received_data_size == full_data_size) 
				{
					cout << received_data << endl;
					received_data = "";
					received_data_size = 0;
					full_data_size = 0;
				}
			} else {
				if (frame[0] < 126)
				{
					full_data_size = frame[0];
				} else {
					int temp = frame[1] * 256;
					full_data_size = ~temp;
					full_data_size += frame[2];
					full_data_size = 257 + full_data_size;
				}

				if (data_size <= 126) 
				{
					frame.erase(frame.begin(), frame.begin() + 1);
				} else {
					frame.erase(frame.begin(), frame.begin() + 3);
				}

				received_data_size += frame.size();
				frame.push_back('\0');
				string str_data = frame.data();
				received_data += str_data;
				if (received_data_size == full_data_size) {
					if (received_data != "ping") {
						PROTOCOL::PACKET packet_data = ParseAndDecrypt(received_data);
						gui.processMessageEvent(conversation, packet_data.senderNick, packet_data.message, gui.inputID);
					}
					received_data = "";
					received_data_size = 0;
					full_data_size = 0;
				}
			}

		}

		ZeroMemory(&buffer, sizeof(buffer));
	}

}

BOOL SendNick() {
	string data;
	data = "{" + Nick + "}";
	if (SendTo(data)) {
		char buffer[512];
		while (true) {
			recv(sock, buffer, sizeof(buffer), 0);
			string b = buffer;
			if (b.find("ping") == string::npos) {
				if (b.find("true") == string::npos) {
					closesocket(sock);
						gui.SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_RED);
						gui.WriteText(3, 24, "Этот ник уже занят", -1, true);
						gui.SetColor(defColor);
						return FALSE;
				}
				else {
					return TRUE;
				}
			}
			ZeroMemory(&buffer, sizeof(buffer));
		}
	}
	else {
		return FALSE;
	}
}

BOOL SendMessageTo(string receiver, string message) {
	string data;
	data = "{" + Nick + "}" + "{" + receiver + "}" + "{" + message + "}";
	if (SendTo(data)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CheckSwichingProtocol() {
	char buffer[512];
	while (true) {
		recv(sock, buffer, sizeof(buffer), 0);
		string b = buffer;
		if (b.find("ping") == string::npos) {
			if (b.find("101 Switching Protocols") == string::npos) {
				closesocket(sock);
				return FALSE;
			}
			else {
				return TRUE;
			}
		}
		ZeroMemory(&buffer, sizeof(buffer));
	}
}

BOOL LoadWSAlib() {
	WSADATA wsaData;

	if (!WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}

BOOL Connect2Server() {

	if (!LoadWSAlib()) {
		return FALSE;
	}

	if (!SetUpConnection()) {
		return FALSE;
	}

	if (!SendHandshake()) {
		return FALSE;
	}

	if (!CheckSwichingProtocol()) {
		return FALSE;
	}

	if (!SendNick()) {
		return FALSE;
	}

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)receiveData, CreateMutex(NULL, FALSE, NULL), 0, NULL);

	return TRUE;
}