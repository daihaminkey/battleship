#pragma once
#include "Field.h"
class ComputerField :
	public Field
{
public:
	ComputerField();
	void GenShipsPlace();//—генерировать корабли на случайных местах
	bool GenShipPlace(int id);//—генерировать корабль
	void ClearField();//ќчистить поле
};

