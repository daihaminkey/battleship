#include "stdafx.h"
#include "Game.h"
#define side 32
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
//v3.2
using namespace std;



Game::Game(Graphics &g)
{

	CompField = ComputerField();
	PlayerField = Field();
	GameState = State::setup;
	G = g;
	selectedShipId = 9;
	lastX = -1;
	lastY = -1;


}

Game::~Game()
{
	G.~Graphics();
}

void Game::DrawTile(int x, int y, bool offset, Tile T)
{
	if (x < 10 && y < 10 && x >= 0 && y >= 0)
	{
		char *path;
		switch (T)
		{

		case Tile::damaged:
			path = "img/dmg.bmp";
			break;
		case Tile::fire:
			path = "img/fire.bmp";
			break;
		case Tile::ship:
			path = "img/ship.bmp";
			break;
		case Tile::miss:
			path = "img/miss.bmp";
			break;
		case Tile::error:
			path = "img/error.bmp";
			break;
		default:
			G.DrawImage("img/bg.bmp", 40 + x*side + (offset ? side * 11 + 32 : 0), 64 + y*side, side*(12 + x) + 40, (2 + y)*side, side, side);
			return;
		}

		G.DrawImage(path, 40 + x*side + (offset ? side * 11 + 32 : 0), 64 + y*side);
	}


}

void Game::DrawRealTile(int x, int y, bool offset)
{
	Field &field =  offset ? PlayerField : CompField;

	if (!field.Cells[x][y].ShipId)
	{
		if (field.Cells[x][y].Shot)
			DrawTile(x, y, offset, Tile::miss);
		else
			DrawTile(x, y, offset, Tile::water);

	}
	else if (field.Ships[field.Cells[x][y].ShipId - 1].Alive())
	{
		if(field.Cells[x][y].Shot)
			DrawTile(x, y, offset, Tile::fire);
		else
			DrawTile(x, y, offset, offset? Tile::ship : Tile::water);
	}
	else
		DrawTile(x, y, offset, Tile::damaged);
}


void Game::DrawGameScreen()
{
	G.DrawImage("img/bg.bmp", 0, 0);

	for(int i = 0; i < 2; ++i)
		for(int x = 0; x < 10; ++x)
			for(int y = 0; y < 10; ++y)
				//if((i ? PlayerField : CompField).Cells[x][y].ShipId)
					DrawRealTile(x, y, i);
}

void Game::MenuClick(int i)
{
	if (i % 3 != 2)
	{
		i += 1;
		i /= 3;
		
		switch (i)
		{
		case 0://Новая игра
			CompField = ComputerField();
			PlayerField = Field();
			GameState = State::setup;
			selectedShipId = 9;
			lastX = -1;
			lastY = -1;
			DrawGameScreen();
			lifeAI = 10;
			lifePlayer = 10;
			break;

		case 1://Сохранить
			if (GameState != State::setup)
				DrawMessage(Save() ? Message::saved : Message::Error);
			else
				DrawMessage(Message::unsaved);
			break;

		case 2://Загрузить
			DrawMessage(Load() ? Message::loaded : Message::Error);
			break;

		case 3://Выход
			exit(0);
			break;
		}
	}
}

void Game::Click(int x, int y, _MOUSE_EVENT_RECORD button)
{
	x = (8.06722689 * (double)x - 40) / 32;
	y = (16.5862069 * (double)y - 66) / 33;
	if (x >= 23 && x <= 27)
		MenuClick(y);
	switch (GameState)
	{
	case State::game:

		if (x < 10 && y < 10 && !CompField.Cells[x][y].Shot)
		{
			Shoot(x, y, true);
			if (lifeAI == 0)
			{
				DrawMessage(Message::win, false);
				GameState = State::end;
			}
		}
		break;

	case State::setup:
		//x = (8.06722689 * (double)x - 40) / 32 - 12;
		//y = (16.5862069 * (double)y - 66) / 33;
		x -= 12;
		if (x >= 0 && x < 10 && y >= 0 && y < 10)
		{
			switch (button.dwButtonState)
			{
			case FROM_LEFT_1ST_BUTTON_PRESSED:
					if(!Ready)
						PlaceShip(x, y);
					break;
			case RIGHTMOST_BUTTON_PRESSED:
					//if (x != lastX || y != lastY)
						RemoveShip(x, y);
					break;
			case FROM_LEFT_2ND_BUTTON_PRESSED:
				PlayerField.Ships[selectedShipId].Horizontal = !PlayerField.Ships[selectedShipId].Horizontal;
				EraseOldShip();
				DrawMouseShip(x, y);
				break;
			}
		}
		break;

	}
	
}

void Game::MouseMove(int x, int y)
{
	switch (GameState)
	{
		case State::setup:
			x = (8.06722689 * (double)x - 40) / 32 - 12;
			y = (16.5862069 * (double)y - 66) / 33;
			if(!Ready)
			if (x != lastX || y != lastY)
			{
				EraseOldShip();
				DrawMouseShip(x, y);
			}
		break;
		
	}
}

void Game::EraseOldShip()
{
	if (redrawable)
	{
		for (int k = 0; k < lastLen; ++k)
		{
			if (lastX >= 0 && lastX < 10 && lastY >= 0 && lastY < 10)
			{
				DrawRealTile(lastX, lastY, true);
				lastHorizontal ? ++lastX : ++lastY;
			}
		}
		(lastHorizontal ? lastX : lastY)-=lastLen;
	}
}

void Game::DrawMouseShip(int x, int y)
{

	if (PlayerField.IsPlaceble(x, y, selectedShipId, false))
	{

		int &len = PlayerField.Ships[selectedShipId].Length;
		bool &horiz = PlayerField.Ships[selectedShipId].Horizontal;


		lastX = x;
		lastY = y;
		lastHorizontal = horiz;
		lastLen = len;
		redrawable = true;

		for (int k = 0; k < len; ++k)
		{
			
			DrawColoredMouse(x, y);
			horiz ? ++x : ++y;
		}
	}
	else
		redrawable = false;
}

void Game::DrawColoredMouse(int x, int y)
{
	(PlayerField.Cells[x][y].ShipId || PlayerField.Cells[x][y].Near) ? DrawTile(x, y, true, Tile::error) : DrawTile(x, y, true, Tile::ship);
}

void Game::DrawShip(int x, int y, bool offset)
{
	Field &f = (offset ? PlayerField : CompField);
	int id = f.Cells[x][y].ShipId-1;
	for (int i = 0; i < f.Ships[id].Length; ++i, (f.Ships[id].Horizontal ? x : y)++)
		DrawTile(x, y, offset, Tile::ship);
}

void Game::PlaceShip(int x, int y)
{
	if (PlayerField.IsPlaceble(x, y, selectedShipId))
	{
		PlayerField.PlaceShip(x, y, selectedShipId);
		DrawShip(x, y, true);
		
		shipPool[selectedShipId] = 0;
		if (!PickFreeShip())
		{
			DrawMessage(Message::confirm);
			Ready = true;
			EraseOldShip();
		}
		
	}
}

void Game::DrawMessage(Message M, bool redraw)
{
	char *path;

	switch (M)
	{
	case saved:
		path = "img/saved.bmp";
		break;
	case loaded:
		path = "img/loaded.bmp";
		break;
	case Error:
		path = "img/err.bmp";
		break;
	case win:
		path = "img/win.bmp";
		break;
	case fail:
		path = "img/fail.bmp";
		break;

	case confirm:
		path = "img/confirmed.bmp";
		break;

	case unsaved:
		path = "img/unsaved.bmp";
		break;

	default: return;
	}
	G.DrawImage(path,134,392);
	if (redraw)
	{
		Sleep(1500);
		G.DrawImage("img/bg.bmp", 134, 392, 134, 392, 478, 74);
	}
}

void Game::RemoveShip(int x, int y)
{
	int id = PlayerField.Cells[x][y].ShipId-1;
	if (id != -1)
	{
		int part = PlayerField.Cells[x][y].PartNum;
		int len = PlayerField.Ships[id].Length;
		bool horiz = PlayerField.Ships[id].Horizontal;
		PlayerField.RemoveShip(horiz ? x - part : x, horiz ? y : y - part);
		for (int i = 0; i < len; ++i)
			DrawTile(horiz ? x - part + i : x, horiz ? y : y - part + i, true, Tile::water);
		//DrawRealTile(horiz ? x - part + i : x, horiz ? y : y - part + i, true);
		shipPool[id] = 1;
		selectedShipId = id;
		DrawMouseShip(x, y);
		Ready = false;
	}
}

bool Game::Shoot(int x, int y, bool player)
{
	Cell &cell = (player ? CompField : PlayerField).Cells[x][y];
	if (cell.ShipId)//попал
	{

		Ship &ship = (player ? CompField : PlayerField).Ships[cell.ShipId - 1];

		if (ship.State[cell.PartNum])
		{
			ship.State[cell.PartNum] = false;
			cell.Shot = true;
			if (ship.Alive())
				DrawTile(x, y, !player, Tile::fire);

			else//убит
			{
				int len = ship.Length;
				int part = cell.PartNum;
				ship.Horizontal ? x -= part : y -= part;
				for (int i = 0; i < len; ++i)
				{
					DrawTile(x, y, !player, Tile::damaged);
					ship.Horizontal ? x++ : y++;
					if (!cell.ShipId)
						ship.Horizontal ? x -= len : y -= len;
					Sleep(100);
					
				}

				int xCond, yCond;
				if (ship.Horizontal)
				{
					x -= len + 1;
					y -= 1;
					xCond = len + 2;
					yCond = 3;
				}
				else
				{
					y -= len + 1;
					x -= 1;
					yCond = len + 2;
					xCond = 3;
				}
				
				for (int i = 0; i < xCond; ++i)
					for (int j = 0; j < yCond; ++j)
					{
						(!player ? PlayerField : CompField).Cells[x + i][y + j].Shot = true;
						if (!(!player ? PlayerField : CompField).Cells[x + i][y + j].ShipId)
						{
							DrawTile(x + i, y + j, !player, Tile::miss);
							//Sleep(10);
						}
					}
				(player ? lifeAI : lifePlayer)--;
			}

		}
		return true;
	}
	else
	{
		cell.Shot = 1;
		DrawTile(x, y, !player, Tile::miss);
	}
	if (player)
		AI();
	return false;
}

bool Game::PickFreeShip()
{
		for (int i = 9; i >= 0; --i)
			if (shipPool[i])
			{
				EraseOldShip();
				selectedShipId = i;
				DrawMouseShip(lastX, lastY);
				return true;
			}
		return false;
}

bool Game::PickShip(int len)
{
	if (PlayerField.Ships[selectedShipId].Length != len)
	{
		for (int i = 0; i < 10; i++)
			if (shipPool[i] && PlayerField.Ships[i].Length == len)
			{
				EraseOldShip();
				selectedShipId = i;
				DrawMouseShip(lastX, lastY);
				return true;
			}
		return false;
	}
	return true;
}

void Game::AI_AddLine(int map[10][10], int x, int y, int l, bool horizont)
{
	for (int i = 0; i < l; ++i)
	{
		if ((horizont ? PlayerField.Cells[x + i][y] : PlayerField.Cells[x][y + i]).Shot)
			return;
	}
	for (int i = 0; i < l; ++i)
		horizont ? map[x + i][y]++ : map[x][y + i]++;
}

void Game::AI_AddCross(int map[10][10], int x, int y)
{
	int price = 20;
	for (int i = 0; i < 5; ++i)
	{
		price *= -1;
		if (i == 2)
			continue;
		if(!PlayerField.Cells[x - 2 + i][y].Shot)
			map[x - 2+i][y] += 60 + price;
		if(!PlayerField.Cells[x][y - 2 + i].Shot)
		map[x][y - 2 + i] += 60 + price;
		
	}
}

bool Game::AI()
{
	//cout << '.';
	Sleep(300);

	int map[10][10];
	for (int x = 0; x < 10; ++x)
		for (int y = 0; y < 10; ++y)
			map[x][y] = 0;

	int lens[10];
	for (int l = 0; l < 10; ++l)
	{
		lens[l] = PlayerField.Ships[l].Alive() ? PlayerField.Ships[l].Length : 0;

		if(lens[l])
			for (int x = 0; x < 10; ++x)
				for (int y = 0; y < 10; ++y)
				{
					if (!PlayerField.Cells[x][y].Shot)
					{
						if (lens[l] == 1)
							map[x][y]++;
						else
						{
							if (x + lens[l] <= 10)
								AI_AddLine(map, x, y, lens[l], true);
							if (y + lens[l] <= 10)
								AI_AddLine(map, x, y, lens[l], false);
						}
					}
				}
	}

	for (int x = 0; x < 10; ++x)
		for (int y = 0; y < 10; ++y)
		{
			if (PlayerField.Cells[x][y].Shot && PlayerField.Cells[x][y].ShipId)
				AI_AddCross(map, x, y);
		}



	int max = 0;

	for (int x = 0; x < 10; ++x)
		for (int y = 0; y < 10; ++y)
			if (max < map[x][y])
				max = map[x][y];


	int x, y;

	do
	{
		x = rand() % 10;
		y = rand() % 10;
	} while (map[x][y] != max);


	if (Shoot(x, y, false))
	{
		if (lifePlayer == 0)
		{
			DrawMessage(Message::fail, false);
			GameState = State::end;
		}
		else
			AI();
	}




	return true;
}

bool Game::Save()
{
	ofstream File("save.dat", ios::binary);
	if (File.is_open())
	{
		File.write((char*)&CompField, sizeof(Field));
		File.write((char*)&PlayerField, sizeof(Field));
		File.close();
		return true;
	}
	return false;
}

bool Game::Load()
{
	ifstream File("save.dat", ios::binary);
	if (File.is_open())
	{
		File.read((char*)&CompField, sizeof(Field));
		File.read((char*)&PlayerField, sizeof(Field));
		File.close();

		GameState = State::game;
		lifeAI = 0;
		lifePlayer = 0;
		for (int i = 0; i < 10; i++)
		{
			if (CompField.Ships[i].Alive())
				lifeAI++;
			if (PlayerField.Ships[i].Alive())
				lifePlayer++;
		}
		DrawGameScreen();
		return true;
	}
	return false;
}