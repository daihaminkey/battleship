#include "stdafx.h"
#include "Ship.h"


using namespace std;

using namespace std;


Ship::Ship(int len)
{
	Length = len;
}

bool Ship::Alive()
{
	for (int i = 0; i < Length; ++i)
		if (State[i])
			return true;
	return false;
}

