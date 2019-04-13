#pragma once

constexpr int MAXPACKETLENGTH = 256;

class PROTOCOL {
private:
public:
	struct PACKET {
		string senderNick;
		string message;
	};

	CHAR * ADDR;
	int PORT;
	SOCKET SOCK;

};
PROTOCOL PROTO;

PROTOCOL::PACKET ParseAndDecrypt(string data) {
	string nick = data.substr(0, 29);
	string message = data.erase(0, 29);
	PROTOCOL::PACKET p;
	p.message = message;
	p.senderNick = nick;
	return p;
}