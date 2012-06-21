#ifndef BINARY_BITMAP_H
#define BINARY_BITMAP_H

#include "Headers/Headers.h"

class CBinaryBitmap
{
public:
	CBinaryBitmap(HBITMAP hBitmap);
	virtual ~CBinaryBitmap();

	unsigned long GetArraySize() const;
	int GetFirstByte(int nX, int nY) const;

	COLORREF GetPixel(int nX, int nY) const;
	void SetPixel(int nX, int nY, COLORREF nColor);

private:
	HBITMAP m_hBitmap;
	BYTE* m_pBits;
	BITMAP m_oBitmapInfo;
};

#endif