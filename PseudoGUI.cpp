
#include "pch.h"
#include "inet_.h" // for inet_pton&inet_ntop in mingw
#include "Auxiliary.h"
#include "ChatGUI.h"
#include "Protocol.h"
#include "Server.h"
#include "Client.h"



int main()
{
	SetConsoleCP(866);
	SetConsoleOutputCP(866);
	SetConsoleTitleA("                    Console chat   0.0.4");
	system("mode con cols=80 lines=25");

	DrawChatGUI();
	
	return 0;
}
