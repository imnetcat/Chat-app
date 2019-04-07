#pragma once

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbi;
WORD defColor;

void SetCurPos(int x, int y) {
	SetConsoleCursorPosition(hStdOut, { (SHORT)x, (SHORT)y });
}

class GUI {
public:
	int drawWindow(int x, int y, int width, int height, string title, WORD color = defColor, WORD borderColor = defColor) {
		if (title.size() + 4 > width) width = title.size() + 4;
		if (height < 2) height = 2;

		SetConsoleTextAttribute(hStdOut, borderColor);
		SetCurPos(x, y);
		cout << "Ú";
		SetCurPos(x + width - 1, y);
		cout << "¿";
		SetCurPos(x, y + height - 1);
		cout << "À";
		SetCurPos(x + width - 1, y + height - 1);
		cout << "Ù";
		SetCurPos(x + 3 + title.size(), y);
		for (int i = 3 + title.size(); i < width - 1; i++) cout << "Ä";
		SetCurPos(x + 1, y + height - 1);
		for (int i = 1; i < width - 1; i++) cout << "Ä";
		for (int i = 1; i < height - 1; i++) {
			SetCurPos(x, y + i);
			cout << "³";
			SetCurPos(x + width - 1, y + i);
			cout << "³";
		}
		SetCurPos(x + 2 + title.size(), y);
		cout << "Ã";

		SetConsoleTextAttribute(hStdOut, color);
		SetCurPos(x + 2, y);
		cout << title;

		elements.push_back({ 0, x, y, width, height, title, "", "", NULL, color, borderColor });
		SetConsoleTextAttribute(hStdOut, defColor);
		return elements.size() - 1;
	}
	int drawButton(int x, int y, string caption, string key, WORD color = defColor, WORD borderColor = defColor) {
		SetConsoleTextAttribute(hStdOut, borderColor);
		SetCurPos(x, y);
		cout << "[";
		SetCurPos(x + 2 + key.size() - 1, y);
		cout << "]";

		SetConsoleTextAttribute(hStdOut, color);
		SetCurPos(x + 1, y);
		cout << key;
		SetCurPos(x + 3 + key.size(), y);
		cout << caption;

		elements.push_back({ 1, x, y, 0, 0, "", caption, key, NULL, color, borderColor });
		SetConsoleTextAttribute(hStdOut, defColor);
		return elements.size() - 1;
	}
	int drawButtonO(int x, int y, string caption, string key, WORD color = defColor, WORD borderColor = defColor) {
		int id = drawButton(x, y, caption, key, color, borderColor);
		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
		SetCurPos(x + 4 + key.size() + caption.size(), y);
		cout << "[®]";

		SetConsoleTextAttribute(hStdOut, defColor);
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
		SetConsoleTextAttribute(hStdOut, color);
		SetCurPos(_x + 4 + _key.size() + _caption.size(), _y);
		cout << "[®]";

		SetConsoleTextAttribute(hStdOut, defColor);
		elements[id].active = active;
		elements[id].type = 2;
		return id;
	}
	int startInput(int x, int y, int width, int height, int key, int id = -1, WORD color = defColor) {
		string caption;
		int _x = x,
			_y = y;
		char ch = NULL;

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

		SetConsoleTextAttribute(hStdOut, color);
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
				}
				else {
					_y--;
					_x = x + width - 1;
				}
				_x++;
			}

			if (ch == 8) {
				if (_x > x) {
					SetCurPos(_x - 1, _y);
					_x--;
				}
				else if (_y > y) {
					SetCurPos(x + width - 1, _y - 1);
					_x = x + width - 1;
					_y--;
				}
				else ch = -1;
				if (ch != -1) {
					cout << " ";
					caption = caption.substr(0, caption.size() - 1);
				}
				SetCurPos(_x, _y);
			}
		}

		if (id == -1) {
			elements.push_back({ 3, x, y, width, height, "", caption, to_string(key), NULL, color, defColor });
			SetConsoleTextAttribute(hStdOut, defColor);
			id = elements.size() - 1;
		}
		else {
			elements[id].caption = caption;
		}
		return id;
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

VOID DrawChatGUI(VOID) {
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
	defColor = csbi.wAttributes;

	GUI gui;
	system("cls");
	gui.drawWindow(0, 0, 30, 10, "Buttons", FOREGROUND_GREEN, FOREGROUND_RED);
	gui.drawWindow(30, 0, 30, 15, "Input", FOREGROUND_GREEN, FOREGROUND_RED);
	gui.drawWindow(0, 10, 30, 5, "Window Name", FOREGROUND_GREEN, FOREGROUND_RED);
	gui.drawButton(2, 3, "Exit", "ECS", defColor, FOREGROUND_RED);
	int button = gui.drawButtonO(2, 4, "Input", "CTRL + I", defColor, FOREGROUND_RED);
	int input = -1;

	SetCurPos(0, 15);
	char res = NULL;
	while (res != 27) {
		res = _getch();
		if (res == 9) {
			gui.buttonOupd(button);
			if (input != -1) gui.startInput(0, 0, 0, 0, 0, input); else input = gui.startInput(31, 1, 28, 13, 9);
			gui.buttonOupd(button);
			SetCurPos(0, 15);
		}
	}
	system("cls");
}