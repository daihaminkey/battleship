#pragma once
#include "Field.h"
class ComputerField :
	public Field
{
public:
	ComputerField();
	void GenShipsPlace();//������������� ������� �� ��������� ������
	bool GenShipPlace(int id);//������������� �������
	void ClearField();//�������� ����
};

