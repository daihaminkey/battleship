#include "stdafx.h"
#include "ComputerField.h"
#include <iostream>

ComputerField::ComputerField():Field()
{
	GenShipsPlace();
}


void ComputerField::ClearField()
{
	for(int x = 0; x < 10; ++x)
		for (int y = 0; y < 10; ++y)
		{
			Cells[x][y].Near = 0;
			Cells[x][y].PartNum = 0;
			Cells[x][y].ShipId = 0;
		}
}

void ComputerField::GenShipsPlace()
{
	bool error;
	do
	{
		error = false;
		for (int i = 0; i < 10; i++)
			error = error || !GenShipPlace(i);
		if (error)
			ClearField();
	} while (error);
	
}

bool ComputerField::GenShipPlace(int id)
{
	int x, y, errorCount = 0;
	
	
	do
	{
		Ships[id].Horizontal = rand() % 2;
		x = rand() % 10;
		y = rand() % 10;
		++errorCount;
		if (errorCount > 100)
			return false;
	}
	while (!IsPlaceble(x, y, id));

	PlaceShip(x, y, id);
	return true;
}