#ifndef BITMAP_H
#define BITMAP_H

#include "Headers/Headers.h"
#include "App.h"

class CBitmap
{
public:
	static HBITMAP Load(const string& sFile);
	static void Save(HBITMAP hBitmap, const string& sFile, HDC hDC);
	static HBITMAP Create(HDC hDC, int nWidth, int nHeight);
};

#endif