#pragma once
class Ship
{
public:
	Ship(int len = 4);

	int Length = 4;
	bool State[4] = { 1,1,1,1 };
	bool Alive();
	bool Horizontal = true;

	
};

