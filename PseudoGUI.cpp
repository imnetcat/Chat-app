
#include "pch.h"
#include "Auxiliary.h"
#include "ChatGUI.h"
#include "Protocol.h"
#include "Peer.h"



int main()
{
	SetConsoleCP(866);
	SetConsoleOutputCP(866);
	SetConsoleTitleA("Console chat ~~~ 0.2.1");
	system("mode con cols=80 lines=25");

	DrawChatGUI();
	
	return 0;
}
