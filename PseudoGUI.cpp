
#include "pch.h"
#include "Auxiliary.h"
#include "SingIn.h"
#include "ChatGUI.h"



int main()
{
	SetConsoleCP(866);
	SetConsoleOutputCP(866);
	SetConsoleTitleA("                    Console chat   0.0.4");
	system("mode con cols=80 lines=25");

	DrawSingIn();

	DrawChatGUI();
	
	return 0;
}
