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

PROTOCOL::PACKET ParseAndDecrypt(string data) { // {Sender_Nick}{Receiver_Nick}{Message}
	size_t pos;

	data.erase(0, 1);
	pos = data.find('}');
	string nick = data.substr(0, pos);

	data.erase(0, pos + 2);
	pos = data.find('}');
	data.erase(0, pos + 2);
	string message = data.substr(0, data.length()-1);

	PROTOCOL::PACKET p;
	p.message = message;
	p.senderNick = nick;
	return p;
}