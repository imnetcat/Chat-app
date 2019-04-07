#pragma once

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbi;
WORD defColor;

void SetCurPos(int x, int y) {
    SetConsoleCursorPosition(hStdOut, {(SHORT)x, (SHORT)y});
}

void WriteText(int x, int y, string text) {
    SetCurPos(x, y);
    cout << text;
}

void SetColor(WORD color) {
    SetConsoleTextAttribute(hStdOut, color);
}

class GUI {
    public:
        int drawWindow(int x, int y, int width, int height, string title, WORD color = defColor, WORD borderColor = defColor) {
            SetColor(borderColor);
            WriteText(x, y, "Ú");
            WriteText(x + width - 1, y, "¿");
            WriteText(x, y + height - 1, "À");
            WriteText(x + width - 1, y + height - 1, "Ù");

            for (int i = 1; i < width - 1; i++) {
                WriteText(x + i, y, "Ä");
                WriteText(x + i, y + height - 1, "Ä");
            }
            for (int i = 1; i < height - 1; i++) {
                WriteText(x, y + i, "³");
                WriteText(x + width - 1, y + i, "³");
            }

            WriteText(x + 1, y, "´");
            WriteText(x + 2 + title.size(), y, "Ã");

            SetColor(color);
            WriteText(x + 2, y, title);

            elements.push_back({0, x, y, width, height, title, "", "", NULL, color, borderColor});
            SetColor(defColor);
            return elements.size() - 1;
        }
        int drawButton(int x, int y, string caption, string key, WORD color = defColor, WORD borderColor = defColor) {
            SetColor(borderColor);
            WriteText(x, y, "[");
            WriteText(x + 2 + key.size() - 1, y, "]");

            SetColor(color);
            WriteText(x + 1, y, key);
            WriteText(x + 3 + key.size(), y, caption);

            elements.push_back({1, x, y, 0, 0, "", caption, key, NULL, color, borderColor});
            SetColor(defColor);
            return elements.size() - 1; 
        }
        int drawButtonO(int x, int y, string caption, string key, WORD color = defColor, WORD borderColor = defColor) {
            int id = drawButton(x, y, caption, key, color, borderColor);
            SetColor(FOREGROUND_RED);
            WriteText(x + 4 + key.size() + caption.size(), y, "[þ]");

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
            WriteText(_x + 4 + _key.size() + _caption.size(), _y, "[þ]");

            SetColor(defColor);
            elements[id].active = active;
            elements[id].type = 2;
            return id;
        }
        int startInput(int x, int y, int width, int height, int key, int id = -1, void (*enter)(void) = NULL, WORD color = defColor) {
            string caption;
            int _x = x,
                _y = y;
            char ch;

            if (id != -1) {
                x = elements[id].x;
                y = elements[id].y;
                width = elements[id].width;
                height = elements[id].height;
                color = elements[id].color;
                caption = elements[id].caption;
                key = stoi(elements[id].key);
                _x = x + caption.size() % width;
                _y = y + caption.size() / width;
            }

            SetColor(color);
            SetCurPos(_x, _y);
            while (ch != key) {
                ch = _getch();

                if (ch >= 32 && ch <= 126) {
                    if (_x > x + width - 1) {
                        _y++;
                        _x = x;
                    }

                    if (_y < y + height) {
                        SetCurPos(_x, _y);
                        cout << ch;
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

                if (ch == 13 && enter) {
                    ch = key;
                    (*enter)();
                }
            }

            if (id == -1) {
                elements.push_back({3, x, y, width, height, "", caption, to_string(key), NULL, color, defColor});
                SetColor(defColor);
                id = elements.size() - 1; 
            } else {
                elements[id].caption = caption;
            }

            return id;
        }
        string getInputText(int id) {
            if (elements[id].caption.size() == 1998913650) return ""; else return elements[id].caption;
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
        vector<Element> elements;
};

bool ENTER;
GUI gui;
int nickname, key;
void enterPress(void) {
    ENTER = true;
    // DO SOMETHING
    // FOR GET INPUT VALUE USE "gui.getInputText(int id);"
    //SAMPLE
    /* if (ERROR) {
        ENTER = false;
        SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_RED);
        WriteText(3, 24, ERROR_TEXT);
        SetColor(defColor);
    } */
}

VOID DrawChatGUI(VOID) {
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    defColor = csbi.wAttributes;

    system("cls");
    system("mode con cols=80 lines=25");
    string window = "sign_in";

    if (window == "sign_in") {
        gui.drawWindow(16, 6, 46, 10, "Sign In", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED, FOREGROUND_GREEN);
        gui.drawButton(43, 14, "Enter", "ENTER", defColor, FOREGROUND_RED);
        gui.drawButton(22, 14, "Switch field", "TAB", defColor, FOREGROUND_RED);
        WriteText(18, 9, "Nickname:");
        WriteText(23, 11, "Key:");
        SetColor(FOREGROUND_RED);
        WriteText(29, 9, "[");
        WriteText(29, 11, "[");
        WriteText(59, 9, "]");
        WriteText(59, 11, "]");
        WriteText(0, 24, "ÄÄ´");
        SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_RED);
        WriteText(3, 24, "READY");
        SetColor(defColor);
        nickname = -1;
        key = -1;


        while (!ENTER) {
            if (nickname != -1) gui.startInput(0, 0, 0, 0, 0, nickname, enterPress); else nickname = gui.startInput(30, 9, 29, 1, 9, -1, enterPress);
            SetCurPos(-1, -1);
            if (!ENTER) if (key != -1) gui.startInput(0, 0, 0, 0, 0, key, enterPress); else key = gui.startInput(30, 11, 29, 1, 9, -1, enterPress);
            SetCurPos(-1, -1);
        }

        ENTER = false;
    }

    window = "main";

    if (window == "main") {
        //****
    }

    system("cls");
}