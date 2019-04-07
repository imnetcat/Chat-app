
#include "pch.h"
#include "Auxiliary.h"
#include "SingIn.h"
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

	DrawSingIn();

	DrawChatGUI();
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Server, CreateMutex(NULL, FALSE, NULL), 0, NULL);

	while (true) {
		CHAR address[512];
		cin >> address;
		cout << "Подкючение к " << address << endl;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Connection, CreateMutex(NULL, FALSE, NULL), 0, NULL);
		Connection(address);
	}
	
	return 0;
}
