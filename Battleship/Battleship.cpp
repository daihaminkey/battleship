#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <conio.h>
#include "Ship.h"
#include <time.h> 
#include "Game.h"
#include <stdio.h>

using namespace std;

int main()
{
	int t = GetTickCount();
	srand(t);
	Graphics graph = Graphics();

	Game G = Game(graph);

	HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
	bool Continue = true;
	INPUT_RECORD input;
	DWORD dwEvents;
	DWORD dwPreviousMode = 0;
	GetConsoleMode(hConsole, &dwPreviousMode);
	DWORD dwNewMode = dwPreviousMode | ENABLE_MOUSE_INPUT;
	dwNewMode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hConsole,dwNewMode|ENABLE_EXTENDED_FLAGS);

	G.DrawGameScreen();
	char inp = 'g';
	while (ReadConsoleInput(hConsole, &input, 1, &dwEvents))
	{
		Sleep(20);
		switch (input.EventType)
		{
		case KEY_EVENT:
			inp = input.Event.KeyEvent.uChar.AsciiChar;
			if (inp == '1' || inp == '2' || inp == '3' || inp == '4')
				G.PickShip(inp-48);
			if (G.Ready && inp == 'e')
				G.GameState = State::game;
			break;
		case MOUSE_EVENT:
			MOUSE_EVENT_RECORD &click = input.Event.MouseEvent;
			if (input.Event.MouseEvent.dwButtonState)
				G.Click(click.dwMousePosition.X, click.dwMousePosition.Y, click);
			else
				G.MouseMove(click.dwMousePosition.X, click.dwMousePosition.Y);
		
			break;
		}

	}

	SetConsoleMode(hConsole, dwPreviousMode | ENABLE_EXTENDED_FLAGS);
	_getch();
	return 0;
}