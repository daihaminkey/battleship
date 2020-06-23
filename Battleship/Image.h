#pragma once
#include <windows.h>

class Image
{
public:
	Image(char path[]);

	~Image();
	int Width, Height;
	HBITMAP Bitmap;
};

