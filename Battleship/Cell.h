#pragma once
class Cell
{
public:
	Cell();

	short ShipId = 0;
	short PartNum = 0;
	bool Shot = false;
	bool Near = false;
};

