#pragma once
#include <algorithm>

constexpr int MAXNICKLENGTH = 30;
string ReceiverNick;
string Nick;
BOOL Connect2Server(VOID);
BOOL SendMessageTo(string, string);

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbi;
WORD defColor;

class GUI {
    public:
        int inputID = -1;
		void SetCurPos(int x, int y) {
			SetConsoleCursorPosition(hStdOut, { (SHORT)x, (SHORT)y });
		}
		void SetColor(WORD color) {
			SetConsoleTextAttribute(hStdOut, color);
		}
		void WriteText(int x, int y, string text, int _inputID = -1, bool debug = false) {
			SetCurPos(x, y);
			if (debug) {
				CONSOLE_SCREEN_BUFFER_INFO _csbi;
				GetConsoleScreenBufferInfo(hStdOut, &_csbi);
				string emptyStr(_csbi.srWindow.Right - (int)text.size() - 3, ' ');
				WORD color = csbi.wAttributes;

				cout << text;
				SetColor(defColor);
				cout << emptyStr;
				SetColor(color);
			} else cout << text;
			if (_inputID != -1) input(_inputID, -1);
		}
        int drawWindow(int x, int y, int width, int height, string title, WORD color = defColor, WORD borderColor = defColor) {
            SetColor(borderColor);
            WriteText(x, y, "┌");
            WriteText(x + width - 1, y, "┐");
            WriteText(x, y + height - 1, "└");
            WriteText(x + width - 1, y + height - 1, "┘");

            for (int i = 1; i < width - 1; i++) {
                WriteText(x + i, y, "─");
                WriteText(x + i, y + height - 1, "─");
            }
            for (int i = 1; i < height - 1; i++) {
                WriteText(x, y + i, "│");
                WriteText(x + width - 1, y + i, "│");
            }

            WriteText(x + 1, y, "┤");
            WriteText(x + 2 + (int)title.size(), y, "├");

            SetColor(color);
            WriteText(x + 2, y, title);

            elements.push_back({ 0, x, y, width, height, title, "", "", NULL, color, borderColor });
            SetColor(defColor);
            return (int)elements.size() - 1;
        }
        int drawButton(int x, int y, string caption, string key, WORD color = defColor, WORD borderColor = defColor) {
            SetColor(borderColor);
            WriteText(x, y, "[");
            WriteText(x + 2 + (int)key.size() - 1, y, "]");

            SetColor(color);
            WriteText(x + 1, y, key);
            WriteText(x + 3 + (int)key.size(), y, caption);

            elements.push_back({ 1, x, y, 0, 0, "", caption, key, NULL, color, borderColor });
            SetColor(defColor);
            return (int)elements.size() - 1;
        }
        int drawButtonO(int x, int y, string caption, string key, WORD color = defColor, WORD borderColor = defColor) {
            int id = drawButton(x, y, caption, key, color, borderColor);
            SetColor(FOREGROUND_RED);
            WriteText(x + 4 + (int)key.size() + (int)caption.size(), y, "[■]");

            SetColor(defColor);
            elements[id].active = false;
            return id;
        }
        int buttonOupd(int id) {
            int _x = elements[id].x,
                _y = elements[id].y;
            string _key = elements[id].key,
                _caption = elements[id].caption;
            bool active = elements[id].active;
            active = !active;

            WORD color;
            if (active) color = FOREGROUND_GREEN; else color = FOREGROUND_RED;
            SetColor(color);
            WriteText(_x + 4 + (int)_key.size() + (int)_caption.size(), _y, "[■]");

            SetColor(defColor);
            elements[id].active = active;
            elements[id].type = 2;
            return id;
        }
        int input(int id, char ch, WORD color = defColor, int x = 0, int y = 0, int width = 0, int height = 0, bool pass = false) {
            string caption;
            int _x = x,
                _y = y;

            if (id != -1) {
                x = elements[id].x;
                y = elements[id].y;
                width = elements[id].width;
                height = elements[id].height;
                color = elements[id].color;
                caption = elements[id].caption;
                if (elements[id].key == "true") pass = true;
                _x = x + (int)caption.size() % width;
                _y = y + (int)caption.size() / width;

                if (_y > y + height - 1) {
                    _x = x + width;
                    _y--;
                }
            }

            SetColor(color);
            SetCurPos(-1, -1);
            SetCurPos(_x, _y);

            if (ch >= 32 && ch <= 126) {
                if (_x > x + width - 1) {
                    _y++;
                    _x = x;
                }

                if (_y < y + height) {
                    SetCurPos(_x, _y);
                    if (!pass) cout << ch; else cout << "*";
                    caption += ch;
                } else {
                    _y--;
                    _x = x + width - 1;
                }

                _x++;
            }

            if (ch == 8) {
                if (_x > x) {
                    SetCurPos(_x - 1, _y); 
                    _x--;
                } else if (_y > y) {
                    SetCurPos(x + width - 1, _y - 1);
                    _x = x + width - 1;
                    _y--;
                } else ch = -1;

                if (ch != -1) {
                    cout << " ";
                    caption = caption.substr(0, caption.size() - 1);
                }

                SetCurPos(_x, _y);
            }

            if (id == -1) {
                string key = "";
                if (pass == true) key = "true";
                elements.push_back({ 3, x, y, width, height, "", caption, key, NULL, color, defColor });
                SetColor(defColor);
                id = (int)elements.size() - 1;
            } else {
                elements[id].caption = caption;
            }

            return id;
        }
        void processMessageEvent(int windowOutputID, string nickname, string message, int _inputID = -1) {
            if (message.size() > 0) {
                int x = elements[windowOutputID].x + 1,
                    y = elements[windowOutputID].y + 1,
                    width = elements[windowOutputID].width - 2,
                    height = elements[windowOutputID].height - 2;

                if ((int)messages.size() < windowOutputID + 1) messages.resize(windowOutputID + 1);
                if ((int)messages[windowOutputID].size() < height + 5) messages[windowOutputID].resize(height + 5);
                if (messages[windowOutputID][0].message.size() == 0) messages[windowOutputID][0].message = "0";

                int startPos = stoi(messages[windowOutputID][0].message) + 1;
                int lines = ((int)message.size() - 1) / width + 1;

                messages[windowOutputID][0].message = to_string(startPos + lines);
                messages[windowOutputID][startPos].message = nickname;
                messages[windowOutputID][startPos].color = FOREGROUND_RED;

                for (int i = 1; i <= lines; i++) {
                    messages[windowOutputID][startPos + i].message = message.substr(width * (i - 1), width);
                    messages[windowOutputID][startPos + i].color = defColor;
                }

                if (startPos + lines > height) {
                    rotate(messages[windowOutputID].begin() + 1, messages[windowOutputID].begin() + startPos + lines - height + 1, messages[windowOutputID].end());
                    messages[windowOutputID][0].message = to_string(height);
                }

                string emptyStr(width, ' ');
                for (int i = 1; i <= height; i++) {
                    
                    WriteText(x, y + i - 1, emptyStr);
                    SetColor(messages[windowOutputID][i].color);
                    WriteText(x, y + i - 1, messages[windowOutputID][i].message);
                }

                SetColor(defColor);
                if (_inputID != -1) input(_inputID, -1);
            }
        }
        string getInputText(int id) {
            if (elements[id].caption.size() == 1998913650) return ""; else return elements[id].caption;
        }
		void clearInput(int id) {
			string emptyStr(elements[id].width, ' ');

			for (int i = 0; i < elements[id].height; i++) {
				WriteText(elements[id].x, elements[id].y + i, emptyStr);
			}

			SetCurPos(elements[id].x, elements[id].y);
			elements[id].caption = "";
		}
    private:
        struct Element {
            int type;
            int x;
            int y;
            int width;
            int height;
            string title;
            string caption;
            string key;
            bool active;
            WORD color;
            WORD borderColor;
        };
        struct Message {
            string message;
            WORD color = defColor;
        };
        vector<Element> elements;
        vector<vector<Message>> messages;
};

GUI gui;

const string password = "1";
constexpr INT ENTER = 13;
constexpr INT ESC = 27;
constexpr INT TAB = 9;
constexpr INT BACKSPACE = 8;

int nickname;
int conversation;

VOID DrawChatGUI(VOID) {
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
	defColor = csbi.wAttributes;

	system("cls");
	system("mode con cols=80 lines=25");
	string window = "sign_in";

	if (window == "sign_in") {
		gui.drawWindow(17, 7, 46, 10, "Sign In", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED, FOREGROUND_GREEN);
		gui.drawButton(44, 15, "Enter", "ENTER", defColor, FOREGROUND_RED);
		gui.drawButton(23, 15, "Switch field", "TAB", defColor, FOREGROUND_RED);
		gui.WriteText(19, 10, "Nickname:");
		gui.WriteText(24, 12, "Key:");
		gui.SetColor(FOREGROUND_RED);
		gui.WriteText(30, 10, "[");
		gui.WriteText(30, 12, "[");
		gui.WriteText(60, 10, "]");
		gui.WriteText(60, 12, "]");
		gui.WriteText(0, 24, "──┤");
		gui.SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_RED);
		gui.WriteText(3, 24, "READY");
		gui.SetColor(defColor);
		gui.SetCurPos(0, 0);
		int key = gui.input(-1, -1, defColor, 31, 12, 29, 1, true);
		nickname = gui.input(-1, -1, defColor, 31, 10, 29, 1);

		char ch;
		string active = "nickname";
		while (window == "sign_in") {
			ch = _getch();

			if (ch == ENTER) {
				string pass = gui.getInputText(key);
				string nick = gui.getInputText(nickname);

				if (nick != "") {
					if (pass != "") {
						if (pass == password) {
							Nick = nick;
							if (Connect2Server()) {
								window = "main"; // SUCCESSFULLY SIGNED IN
							}
						}
						else {
							gui.SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_RED);
							gui.WriteText(3, 24, "Пароль неверный", -1, true);
							gui.SetColor(defColor);
						}
					}
					else {
						gui.SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_RED);
						gui.WriteText(3, 24, "Введите пароль", -1, true);
						gui.SetColor(defColor);
					}
				}
				else {
					gui.SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_RED);
					gui.WriteText(3, 24, "Введите никнейм", -1, true);
					gui.SetColor(defColor);
				}
			}

			if (ch == TAB) {
				if (active == "nickname") active = "key"; else active = "nickname";
			}

			if (active == "nickname")
				gui.input(nickname, ch);
			else if (active == "key")
				gui.input(key, ch);
		}
	}

	system("cls");

	if (window == "main") {
		gui.drawWindow(0, 0, 30, 24, "Menu", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED, FOREGROUND_GREEN);
		conversation = gui.drawWindow(30, 0, 50, 19, "Conversation", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED, FOREGROUND_GREEN);
		gui.drawWindow(30, 19, 50, 5, "Input", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED, FOREGROUND_GREEN);
		gui.drawButton(2, 2, "Send", "ENTER", defColor, FOREGROUND_RED);
		gui.drawButton(2, 21, "Exit", "ESC", FOREGROUND_RED, FOREGROUND_RED);
		gui.SetColor(FOREGROUND_RED);
		gui.WriteText(0, 24, "──┤");
		gui.SetColor(defColor);
		gui.WriteText(3, 24, "Ваш ник в системе: "+Nick, -1, true);
		gui.SetCurPos(0, 0);
		int message = gui.input(-1, -1, defColor, 31, 20, 48, 3);
        gui.inputID = message;

		char ch;
		while (window == "main") {
			ch = _getch();

			if (ch == ENTER) {
				string inputText = gui.getInputText(message);

				if (inputText[0] == '/') { // Введена комманда
					size_t pos;
					if ((pos = inputText.find("/r ")) != string::npos) {
						ReceiverNick = inputText.substr(3, inputText.length());
						gui.processMessageEvent(conversation, "", "Получатель установлен: " + ReceiverNick);
					}
				} else { // Введено сообщение
					if (SendMessageTo(ReceiverNick, inputText)) {
						gui.processMessageEvent(conversation, Nick, inputText);
					}
					else {
						gui.processMessageEvent(conversation, "", "Сообщение небыло отправлено! SendTo() error");
					}
				}
				gui.clearInput(message);
            }

            if (ch == ESC) {
				break;
			}

            gui.input(message, ch);
        }
    }

    system("cls");
}