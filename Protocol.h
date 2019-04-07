#pragma once


CONST INT COMMANDKEY = 0;
CONST INT PINGPONG = 1;
CONST INT DOWNLOADFILE = 2;
CONST INT FILENAME = 3;
CONST INT FILESIZE = 4;
CONST INT LOADFILE = 5;
CONST INT HID = 12;
CONST INT LOG = 16;


CONST INT BUFSIZE = 5120;

CHAR buf[BUFSIZE];



// ---------------------------


class PROTOCOL {
private:
	struct COMMANDS {
		bool isCommandkey = false;
		bool isPong = false;
		bool isDownloadFile = false;
		bool isLoadFile = false;
		bool isFileSize = false;
		bool isFileName = false;
		bool charSeparation = false;
		int downloaded = 0;
		int loaded = 0;
		string transferedFileName = "";
		string FileSize = "";
		string separatedChar = "";
		string separatedKey = "";
	};
public:

	void instantActionsSwitch()
	{
		

	}
	void longActionsSwitch(vector <int> intData, string chData)
	{
		
	}

	
	CHAR * ADDR;
	PCSTR PORT = "1212";
	SOCKET SOCK;
	int connections = 0;                            // количество подключений к серверу
	vector <SOCKET> clientSock;
	vector <COMMANDS> globalCommands;

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



VOID WINAPI ParseAndDecrypt(string temp) {


	return;
}