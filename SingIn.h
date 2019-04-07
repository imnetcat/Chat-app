#pragma once

constexpr INT NICKFIELD = 0;
constexpr INT KEYFIELD = 1;
constexpr INT ENTER = 13;
constexpr INT TAB = 9;
constexpr INT BACKSPACE = 8;

class SINGIN {
public:
	int currentInputField = 0;
	string InfoBuf;
	vector <char> NickBuf;
	vector <char> KeyBuf;
	BOOL CompareKeys() {
		KeyBuf.push_back('\0');
		string k = KeyBuf.data();
		KeyBuf.erase(KeyBuf.end() - 1);
		if (k == key) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
private:
	const string key = "1";
};

VOID DrawSingIn(VOID) {
	SINGIN SingIn;
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	while (true) {
		int size = SingIn.NickBuf.size();
		string Nick;
		for (int n = 0; n < 28; n++) {
			if (n < size) {
				Nick += SingIn.NickBuf[n];
			}
			else {
				Nick += ' ';
			}
		}
		size = SingIn.KeyBuf.size();
		string Key;
		for (int n = 0; n < 28; n++) {
			if (n < size) {
				Key += '*';
			}
			else {
				Key += ' ';
			}
		}
		size = SingIn.InfoBuf.size();
		string Info;
		for (int n = 0; n < 80; n++) {
			if (n < size) {
				Info += SingIn.InfoBuf[n];
			}
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		cout << "                 ÚÄÄÄ´Sing inÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                 ";
		cout << "                 ³                                            ³                 ";
		cout << "                 ³                                            ³                 ";
		cout << "                 ³ Nick:  ["   << Nick.c_str() <<     "]      ³                 ";
		cout << "                 ³                                            ³                 ";
		cout << "                 ³ Key:   ["   << Key.c_str() <<      "]      ³                 ";
		cout << "                 ³                                            ³                 ";
		cout << "                 ³                                            ³                 ";
		cout << "                 ³           [TAB] Change Fields              ³                 ";
		cout << "                 ³                                            ³                 ";
		cout << "                 ³  [ENTER] Enter          [BACKSPACE] Clear  ³                 ";
		cout << "                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                 ";
		cout << "                                                                                ";
		cout << "                                                                                ";
		cout << "                                                                                ";
		cout << "                                                                                ";
		cout << "                                                                                ";
		cout << "                                                                                ";
		cout << "ÄÄÄ´";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 64);
		cout << Info.c_str();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFFAULTCOLOR);

		int c;
		int extended = 0;
		c = _getch();
		if (!c)
			extended = _getch();

		SingIn.InfoBuf = "";

		switch (c)
		{
		case ENTER:
			if (SingIn.NickBuf.size() == 0) {
				SingIn.InfoBuf = "“ª ¦¨â¥ ­¨ª";
				break;
			}
			if (SingIn.KeyBuf.size() == 0) {
				SingIn.InfoBuf = "“ª ¦¨â¥ ª«îç";
				break;
			}
			if (SingIn.CompareKeys()) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFFAULTCOLOR);
				return;
			}
			else {
				SingIn.InfoBuf = "Š«îç ­¥ ¢¥à¥­?";
			}
			break;
		case TAB:
			if (SingIn.currentInputField == 1) {
				SingIn.currentInputField = 0;
			}
			else {
				SingIn.currentInputField += 1;
			}
			break;
		case BACKSPACE:
			switch (SingIn.currentInputField)
			{
			case NICKFIELD:
				if (SingIn.NickBuf.size() != 0) {
					SingIn.NickBuf.erase(SingIn.NickBuf.end() - 1);
				}
				break;
			case KEYFIELD:
				if (SingIn.KeyBuf.size() != 0) {
					SingIn.KeyBuf.erase(SingIn.KeyBuf.end() - 1);
				}
				break;
			}
			break;
		default:
			if (extended) {
				SingIn.InfoBuf = "Š« ¢¨è¨ ­¥ ¯®¤¤¥à¦¨¢ îâìáï";
			}
			else {
				switch (SingIn.currentInputField)
				{
				case NICKFIELD:
					if (SingIn.NickBuf.size() < 28) {
						SingIn.NickBuf.push_back(c);
					}
					break;
				case KEYFIELD:
					if (SingIn.KeyBuf.size() < 28) {
						SingIn.KeyBuf.push_back(c);
					}
					break;
				}
			}
			break;
		}
		cout << endl << endl << endl << endl<< endl << endl << endl << endl << endl;
	}
}