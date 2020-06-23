#pragma once
#include "Cell.h"
#include "Ship.h"
class Field
{
public:

	Cell Cells[10][10];
	Ship Ships[10];

	Field();
	void PrintText(int i);//���������� ����� ���������� � ����
	
	void SetNear(int x, int y);//��������� ����� near ��� ������ ������-�������
	void ClearNear();//������� ������ ������
	void ClearNear(int x, int y);//������� ������ ������ ����� � �������

	void GenShips();//��������� �������� ������ �����
	bool IsPlaceble(int x, int y, int shipId, bool near=true);//��������, ����� �� ���������� ������� � ������
	void PlaceShip(int x, int y, int shipId);//������ ������� � ������
	void RemoveShip(int x, int y);//�������� ������� �� �����
	
};

