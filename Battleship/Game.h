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

	ComputerField CompField;//Поле компьютера
	Field PlayerField;//Поле игрока
	Graphics G;//Графика

	State GameState;//Состояние игры

	bool shipPool[10] = { 1,1,1,1,1,1,1,1,1,1 };//Доступные для установки игроком корабли
	bool Ready = false;//Подтверждение расстановки
	
	bool PickShip(int len);//Выбрать корабль по длине
	void DrawGameScreen();//Отрисовать весь экран

	void Click(int x, int y, _MOUSE_EVENT_RECORD button);//Обработчик клика
	void MouseMove(int x, int y);//Обработчик мыши
	
private:
	int lastX, lastY, lastLen, selectedShipId;//переменные состояний
	bool lastHorizontal, redrawable;//переменные состояний
	int lifeAI = 10;//осталось жизней компьютера
	int lifePlayer = 10;//осталось жизней игрока

	bool Save();//Сохранение
	bool Load();//Загрузка
	bool AI();//Ход компьютера
	void AI_AddLine(int map[10][10], int x, int y, int l, bool horizont);//расстановка приоритета стрельбы
	void AI_AddCross(int map[10][10], int x, int y);//расстановка приоритета стрельбы

	bool Shoot(int x, int y, bool player);//выстрел
	void PlaceShip(int x, int y);//расположить корабль
	void RemoveShip(int x, int y);//удалить корабль

	
	bool PickFreeShip();//Выбрать свободный корабль

	void DrawTile(int x, int y, bool offset, Tile T);//Отрисовать клетку по текстуре
	void DrawRealTile(int x, int y, bool offset);//Отрисовать клетку по тому, что там должно быть
	void DrawShip(int x, int y, bool offset);//Отрисовать корабль
	void DrawMouseShip(int x, int y);//Отрисовать корабль по курсору мыши
	void EraseOldShip();//Стереть корабль курсора прошлого фрейма
	void DrawColoredMouse(int x, int y);//Нарисовать красный корабль
	void DrawMessage(Message D, bool redraw = true);//Вывести сообщение
	void MenuClick(int i);//Обработчик клика по меню
};

