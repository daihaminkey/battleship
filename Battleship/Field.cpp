#include "stdafx.h"
#include "Field.h"
#include <iomanip>
#include <conio.h>
#include <iostream>


using namespace std;

Field::Field()
{
	GenShips();
	for(int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			Cells[i][j] = Cell();
}

void Field::PrintText(int i)
{
	char *ch;
	switch (i)
	{
	case 1:
		ch = "parts";
		break;

	case 2:
		ch = "near";
		break;

	default:
		ch = "map";
		break;
	}


	cout << ch <<": " << endl;
	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 10; ++x)
		{
			if(i == 0)
				cout << setw(3) << Cells[x][y].ShipId;
			else if(i == 1)
				cout << setw(3) << Cells[x][y].PartNum;
			else
				cout << setw(3) << Cells[x][y].Near;
		}
		cout << endl;
	}
}

void Field::GenShips()
{
	int Max = 4;
	int Current = 0;
	int len = 1;
	for (int i = 0; i < 10; i++)
	{
		if (Current == Max)
		{
			Max--;
			Current = 0;
			len++;
		}
		Ships[i] = Ship(len);
		Current++;
	}
}


void Field::SetNear(int x, int y)
{
	for(int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
		{
			int pointX = x + i;
			int pointY = y + j;

			if (pointX >= 0 && pointX < 10 && pointY >= 0 && pointY < 10)
				if (!Cells[pointX][pointY].ShipId)
					Cells[pointX][pointY].Near = true;
		}
}

void Field::ClearNear()
{
	for (int y = 0; y < 10; ++y)
		for (int x = 0; x < 10; ++x)
			if (Cells[x][y].ShipId &&Cells[x][y].Near)
				Cells[x][y].Near = false;

	
}

void Field::ClearNear(int x, int y)
{
	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
			if (x + i >= 0 && x + i < 10 && y + j >= 0 && y + j < 10)
				if (Cells[x+i][y+j].ShipId)
					return;

	Cells[x][y].Near = false;
}


bool Field::IsPlaceble(int x, int y, int shipId, bool near)
{
	Ship &ship = Ships[shipId];
	bool ret = false;
	if (x >= 0 && x <= 10 - (ship.Horizontal ? ship.Length : 1) && y >= 0 && y <= 10 - (ship.Horizontal ? 1 : ship.Length))
	{
		for (int k = 0; k < ship.Length; ++k)
		{
			ret = ret || Cells[x][y].ShipId || (near ? Cells[x][y].Near : false);
			ship.Horizontal ? ++x : ++y;
		}
		return !ret;
	}
	return false;
}

void Field::PlaceShip(int x, int y, int shipId)
{
	Ship &ship = Ships[shipId];
	for (int k = 0; k < ship.Length; ++k)
	{
		Cells[x][y].ShipId = shipId+1;
		Cells[x][y].PartNum = k;
		SetNear(x, y);
		ship.Horizontal ? ++x : ++y;
	}
	ClearNear();

}



void Field::RemoveShip(int x, int y)
{
	int len = Ships[Cells[x][y].ShipId - 1].Length;
	bool horiz = Ships[Cells[x][y].ShipId - 1].Horizontal;
	for (int i = 0; i < len; ++i)
	{
		Cells[horiz ? x+i : x][horiz ? y : y+i].ShipId = 0;
		Cells[horiz ? x+i : x][horiz ? y : y+i].PartNum = 0;
	}

	for (int i = -1; i <= len; ++i)
		for(int j = -1; j < 2; ++j)
			ClearNear(horiz ? x + i : x+j, horiz ? y+j : y + i);
	
}
