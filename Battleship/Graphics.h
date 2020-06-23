#pragma once
#include "Image.h"
class Graphics
{
public:
	Graphics();
	~Graphics();

	void DrawImage(Image I, int x, int y);
	void DrawImage(char *path, int x, int y);
	void DrawImage(Image I, int x, int y, int picX, int picY, int width, int height);
	void DrawImage(char *path, int x, int y, int picX, int picY, int width, int height);

private:
	HWND Console;
	HDC DC;
	HDC imageDC;
	HANDLE out;

};

