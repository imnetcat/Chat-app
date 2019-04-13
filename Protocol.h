#pragma once


CONST INT COMMANDKEY = 0;
CONST INT GETNICK = 8;
CONST INT NICK = 9;
CONST INT MESSAGE = 10;

CONST INT BUFSIZE = 5120;

CHAR buf[BUFSIZE];

class PROTOCOL {
private:
	struct COMMANDS {
		bool isCommandkey = false;
		bool isCharSeparation = false;
		bool isGetMessage = false;
		bool isShowMessage = false;
		bool isNick = false;
		bool isGetRemoteNick = false;
		string message = "";
		string remoteNick = "";
		string separatedChar = "";
		string separatedKey = "";
	};
	
public:
	struct CONNECTION {
		SOCKET clientSock;
		COMMANDS globalCommands;
		string ip;
		string nick;
	};

	CHAR * ADDR;
	int PORT;
	SOCKET SOCK;
	int connections = 0;                            // количество подключений к серверу
	vector <CONNECTION> connection;

	void instantActionsSwitch(int j)
	{
		if (connection[j].globalCommands.isShowMessage) {
			gui.processMessageEvent(conversation, connection[j].globalCommands.remoteNick, connection[j].globalCommands.message);
			connection[j].globalCommands.remoteNick = "";
			connection[j].globalCommands.message = "";
			connection[j].globalCommands.isShowMessage = false;
		}
	}
	void longActionsSwitch(vector <int> intData, string chData, int j)
	{
		if (connection[j].globalCommands.isGetMessage) {
			connection[j].globalCommands.message += chData;
		}
		if (connection[j].globalCommands.isGetRemoteNick) {
			connection[j].nick += chData;
		}
	}

};
PROTOCOL PROTO;


INT SendHeader(CONST INT header, SOCKET Sock)
{
	string t = toStrA(header);
	string str = std::string("}") + t + std::string("{ ");
	int ret = send(Sock, (char *)str.c_str(), strlen((char *)str.c_str()), 0);
	return 0;
}
INT Send(CHAR *buffer, SOCKET Sock)
{
	int size = strlen(buffer);
	cout << buffer << endl;
	if (size != 0) {
		string strbuf;
		for (int n = 0; n < size; n++) {
			strbuf += toStrA(buffer[n]);
			strbuf += " ";
		}
		strbuf = strbuf;
		int ret = send(Sock, (char*)strbuf.c_str(), strbuf.size(), 0);
	}
	return 0;
}
INT SendBinary(std::vector <int> buffer, SOCKET Sock)
{
	string buf;
	for (int n = 0; n < buffer.size(); n++) {
		buf += toStrA(buffer[n]);
		buf += " ";
	}
	buf = buf;
	int ret = send(Sock, (char*)buf.c_str(), buf.size(), 0);
	return 0;
}

VOID Send_Message(string  message, string from, string to) {
	int j;
	for (j = 0; j < PROTO.connection.size(); j++) {
		if (PROTO.connection[j].nick == to) {
			break;
		}
	}
	if (to == "") j = 0;
	j = PROTO.connection[j].clientSock;
	SendHeader(NICK, j);
	Send((CHAR*)from.c_str(), j);
	SendHeader(NICK, j);
	SendHeader(MESSAGE, j);
	Send((CHAR*)message.c_str(), j);
	SendHeader(MESSAGE, j);
}

VOID WINAPI ParseAndDecrypt(string temp, int j) {
	std::string delimiter1 = "}";
	std::string delimiter2 = "{";
	int n = 0;
	int i = 0;
	std::vector <int> flags;
	size_t pos1 = 0;
	size_t pos2 = 0;
	std::vector <std::string> fg;
	std::vector <std::string> vecstr;
	pos1 = temp.find(delimiter1);
	pos2 = temp.find(delimiter2);
	flags.push_back(-1);
	if (pos1 != -1) {
		while ((pos1 = temp.find(delimiter1)) != std::string::npos)
		{
			fg.push_back(temp.substr(0, pos1));
			temp.erase(0, pos1 + delimiter1.length());
			n++;
		}
		flags[0] = 0;
		fg.push_back(temp);
		std::string * tmp = fg.data();
		n++;
		if (n >= 2)
		{
			while (i < n) {
				flags[0] = -1;
				std::string strThatContainsCommand = tmp[i];
				pos1 = strThatContainsCommand.find(delimiter2);
				if (pos1 != -1) {
					std::string tm = strThatContainsCommand.substr(0, pos1);
					//vecstr.push_back(tm);
					flags.push_back(toIntA((char*)tm.c_str()));
					strThatContainsCommand.erase(0, pos1 + delimiter1.length());
					pos1 = strThatContainsCommand.find(delimiter2);
					tm = strThatContainsCommand.substr(0, pos1);
					vecstr.push_back(tm);
				}
				else {
					vecstr.push_back(strThatContainsCommand);
				}
				i++;
			}
		}
		else {
			vecstr.push_back(fg.data()[0]);
		}
	}
	else if (pos2 != -1) {
		while ((pos2 = temp.find(delimiter2)) != std::string::npos)
		{
			fg.push_back(temp.substr(0, pos2));
			temp.erase(0, pos2 + delimiter2.length());
			n++;
		}
		flags[0] = -1;
		fg.push_back(temp);
		std::string * tmp = fg.data();
		n++;
		if (n >= 2)
		{
			while (i < n) {
				flags[0] = 0;
				std::string strThatContainsCommand = tmp[i];
				pos2 = strThatContainsCommand.find(delimiter1);
				if (pos2 != -1) {
					std::string tm = strThatContainsCommand.substr(0, pos2);
					flags.push_back(toIntA((char*)tm.c_str()));
					strThatContainsCommand.erase(0, pos2 + delimiter2.length());
					pos2 = strThatContainsCommand.find(delimiter1);
					tm = strThatContainsCommand.substr(0, pos2);
					vecstr.push_back(tm);
				}
				else {
					vecstr.push_back(strThatContainsCommand);
				}
				i++;
			}
		}
		else {
			vecstr.push_back(fg.data()[0]);
		}
	}
	else {

		vecstr.push_back(temp);

	}

	int c = 0;
	n = 0;
	i = 0;
	int si = flags.size();
	if (flags[0] == -1 && PROTO.connection[j].globalCommands.isCommandkey) {
		PROTO.connection[j].globalCommands.isCommandkey = false;
		flags.insert(flags.begin(), toIntA((char*)PROTO.connection[j].globalCommands.separatedKey.c_str()));
		PROTO.connection[j].globalCommands.separatedKey = "";
		si++;
	}
	n = 0;
	while (n < si) {
		switch (flags[n])
		{
		case COMMANDKEY:
			PROTO.connection[j].globalCommands.isCommandkey = true;
			PROTO.connection[j].globalCommands.separatedKey += vecstr[vecstr.size() - 1];
			break;
		case MESSAGE:
			if (PROTO.connection[j].globalCommands.isGetMessage){
				PROTO.connection[j].globalCommands.isShowMessage = true;
			}
			PROTO.connection[j].globalCommands.isGetMessage = !(PROTO.connection[j].globalCommands.isGetMessage);
			break;
		case NICK:
			PROTO.connection[j].globalCommands.isNick = !(PROTO.connection[j].globalCommands.isNick);
			break;
		case GETNICK:
			PROTO.connection[j].globalCommands.isGetRemoteNick = !(PROTO.connection[j].globalCommands.isGetRemoteNick);
			break;
		}

		if ((i != si || (i == si || si == 0)) && !vecstr.empty()) {
			if (i < vecstr.size() && vecstr[i].size() > 1) {

				std::string stringWithIndexI = vecstr[i];
				std::string delimiter2 = " ";
				size_t pos = 0;
				std::vector <std::string> strData;
				pos = stringWithIndexI.find(delimiter2);
				int tps = pos;
				if (pos = stringWithIndexI.find(delimiter2) != -1) {
					while ((pos = stringWithIndexI.find(delimiter2)) != std::string::npos) {
						strData.push_back((stringWithIndexI.substr(0, pos)));
						stringWithIndexI.erase(0, pos + delimiter2.length());
					}

					if (strData[0] == "" || strData[0].empty()) {
						strData.erase(strData.begin());
					}
				}

				if (tps == 0 && PROTO.connection[j].globalCommands.isCharSeparation) {
					strData.insert(strData.begin(), PROTO.connection[j].globalCommands.separatedChar);
					PROTO.connection[j].globalCommands.isCharSeparation = false;
					PROTO.connection[j].globalCommands.separatedChar = "";
				}
				if (PROTO.connection[j].globalCommands.isCharSeparation) {
					PROTO.connection[j].globalCommands.separatedChar += strData[0];
					strData[0] = PROTO.connection[j].globalCommands.separatedChar;
					PROTO.connection[j].globalCommands.isCharSeparation = false;
					PROTO.connection[j].globalCommands.separatedChar = "";
				}

				if (!stringWithIndexI.empty()) {
					if (stringWithIndexI[0] != '\0') {
						PROTO.connection[j].globalCommands.isCharSeparation = true;
						std::string tempstr = stringWithIndexI;
						size_t pos = 0;
						std::string delimiter = "";
						delimiter += '\0';
						pos = tempstr.find(delimiter);
						PROTO.connection[j].globalCommands.separatedChar += tempstr.substr(0, pos);
					}
					else {
						PROTO.connection[j].globalCommands.isCharSeparation = false;
						PROTO.connection[j].globalCommands.separatedChar = "";
					}
				}
				else {
					PROTO.connection[j].globalCommands.isCharSeparation = false;
					PROTO.connection[j].globalCommands.separatedChar = "";
				}

				std::vector <int> intData;
				for (int nn = 0; nn < strData.size(); nn++) {
					intData.push_back(toIntA((char*)strData[nn].c_str()));
				}
				std::string chData;
				for (int nn = 0; nn < strData.size(); nn++) {
					chData += (char)toIntA((char*)strData[nn].c_str());
				}

				PROTO.longActionsSwitch(intData, chData, j);

			}
			else {
				PROTO.instantActionsSwitch(j);
			}
		}

		i++;
		n++;
	}
	return;
}