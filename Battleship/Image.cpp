#include "stdafx.h"
#include "Image.h"

Image::Image(char path[])
{
	Bitmap = (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	
	BITMAP  bitmapInfo;
	GetObject(Bitmap, sizeof(BITMAP), &bitmapInfo);

	Width = bitmapInfo.bmWidth;
	Height = bitmapInfo.bmHeight;
}


Image::~Image()
{
	DeleteObject(Bitmap);
}
