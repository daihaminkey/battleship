#pragma once
#include "Graphics.h"
#include "ComputerField.h"

enum Tile {  ship, fire, damaged, water, miss, error };
enum State { setup, game, end};
enum Message { saved, loaded, Error, win, fail, confirm, unsaved};

class Game
{
public:
	Game(Graphics &g);
	~Game();

	ComputerField CompField;//���� ����������
	Field PlayerField;//���� ������
	Graphics G;//�������

	State GameState;//��������� ����

	bool shipPool[10] = { 1,1,1,1,1,1,1,1,1,1 };//��������� ��� ��������� ������� �������
	bool Ready = false;//������������� �����������
	
	bool PickShip(int len);//������� ������� �� �����
	void DrawGameScreen();//���������� ���� �����

	void Click(int x, int y, _MOUSE_EVENT_RECORD button);//���������� �����
	void MouseMove(int x, int y);//���������� ����
	
private:
	int lastX, lastY, lastLen, selectedShipId;//���������� ���������
	bool lastHorizontal, redrawable;//���������� ���������
	int lifeAI = 10;//�������� ������ ����������
	int lifePlayer = 10;//�������� ������ ������

	bool Save();//����������
	bool Load();//��������
	bool AI();//��� ����������
	void AI_AddLine(int map[10][10], int x, int y, int l, bool horizont);//����������� ���������� ��������
	void AI_AddCross(int map[10][10], int x, int y);//����������� ���������� ��������

	bool Shoot(int x, int y, bool player);//�������
	void PlaceShip(int x, int y);//����������� �������
	void RemoveShip(int x, int y);//������� �������

	
	bool PickFreeShip();//������� ��������� �������

	void DrawTile(int x, int y, bool offset, Tile T);//���������� ������ �� ��������
	void DrawRealTile(int x, int y, bool offset);//���������� ������ �� ����, ��� ��� ������ ����
	void DrawShip(int x, int y, bool offset);//���������� �������
	void DrawMouseShip(int x, int y);//���������� ������� �� ������� ����
	void EraseOldShip();//������� ������� ������� �������� ������
	void DrawColoredMouse(int x, int y);//���������� ������� �������
	void DrawMessage(Message D, bool redraw = true);//������� ���������
	void MenuClick(int i);//���������� ����� �� ����
};

