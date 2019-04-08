#pragma once

void getAllChars() {
	for (int n = 0; n < 256; n++) {
		cout << n << ": " << (char)n << endl;
	}
	/*
	:
	179: ³
    180: ´
    191: ¿
    192: À
    193: Á
    194: Â
    195: Ã
    196: Ä
    197: Å
	217: Ù
    218: Ú

	*/
}


WORD GetDefColor() {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	return info.wAttributes;
}

CONST WORD DEFFAULTCOLOR = GetDefColor();

void getAllColors() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFFAULTCOLOR);
	for (int n = 0; n < 256; n++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
		cout << n << ": " << "ABCDEFGH" << endl;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFFAULTCOLOR);
}
