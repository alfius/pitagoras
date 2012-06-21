#include "BinaryBitmap.h"

CBinaryBitmap::CBinaryBitmap(HBITMAP hBitmap)
{
	m_hBitmap = hBitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &m_oBitmapInfo);

	m_pBits = new BYTE[GetArraySize()];
	GetBitmapBits(m_hBitmap, GetArraySize(), m_pBits);
}

CBinaryBitmap::~CBinaryBitmap()
{
	SetBitmapBits(m_hBitmap, GetArraySize(), m_pBits);
	delete [] m_pBits;
	m_pBits = 0;
}

unsigned long CBinaryBitmap::GetArraySize() const
{
	return m_oBitmapInfo.bmBitsPixel * m_oBitmapInfo.bmWidth * m_oBitmapInfo.bmHeight;
}

int CBinaryBitmap::GetFirstByte(int nX, int nY) const
{
	return nY * m_oBitmapInfo.bmWidthBytes + nX * m_oBitmapInfo.bmBitsPixel/8;
}

COLORREF CBinaryBitmap::GetPixel(int nX, int nY) const
{
	const int nByte0 = GetFirstByte(nX, nY);

	return 
		RGB
		(
			m_pBits[nByte0 + 2],
			m_pBits[nByte0 + 1],
			m_pBits[nByte0]
		);
}

void CBinaryBitmap::SetPixel(int nX, int nY, COLORREF nColor)
{
	const int nByte0 = GetFirstByte(nX, nY);

	m_pBits[nByte0] = GetBValue(nColor);
	m_pBits[nByte0 + 1] = GetGValue(nColor);
	m_pBits[nByte0 + 2] = GetRValue(nColor);
	
	if (m_oBitmapInfo.bmBitsPixel == 32)
		m_pBits[nByte0 + 3] = 255;
}

