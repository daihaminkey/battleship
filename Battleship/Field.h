#pragma once
#include "Cell.h"
#include "Ship.h"
class Field
{
public:

	Cell Cells[10][10];
	Ship Ships[10];

	Field();
	void PrintText(int i);//отладочный вывод информации о поле
	
	void SetNear(int x, int y);//”становка флага near при св€зке €чейка-корабль
	void ClearNear();//ќчистка лишних флагов
	void ClearNear(int x, int y);//ќчистка лишних флагов р€дом с клеткой

	void GenShips();//√енераци€ кораблей нужной длины
	bool IsPlaceble(int x, int y, int shipId, bool near=true);//ѕроверка, можно ли установить корабль в €чейки
	void PlaceShip(int x, int y, int shipId);//«апись корабл€ в €чейки
	void RemoveShip(int x, int y);//”даление корабл€ из €чеек
	
};

