#include "stdafx.h"
#include "Graphics.h"
#include <iostream>

//using namespace std;

Graphics::Graphics()
{
	Console = GetConsoleWindow();
	DC = GetDC(Console);
	
	out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
	
}



Graphics::~Graphics()
{
	ReleaseDC(Console, DC);
}

void Graphics::DrawImage(Image I, int x, int y)
{
	imageDC = CreateCompatibleDC(NULL);;
	SelectObject(imageDC, I.Bitmap);
	BitBlt(DC, x, y, I.Width, I.Height, imageDC, 0, 0,SRCCOPY);
}

void Graphics::DrawImage(Image I, int x, int y, int picX, int picY, int width, int height)
{
	imageDC = CreateCompatibleDC(NULL);;
	SelectObject(imageDC, I.Bitmap);
	BitBlt(DC, x, y, width, height, imageDC, picX, picY, SRCCOPY);
}

void Graphics::DrawImage(char *path, int x, int y)
{
	DrawImage(Image(path), x, y);
}

void Graphics::DrawImage(char *path, int x, int y, int picX, int picY, int width, int height)
{
	DrawImage(Image(path), x, y,picX,picY,width,height);
}