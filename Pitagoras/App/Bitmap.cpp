#include "Bitmap.h"

HBITMAP CBitmap::Load(const string& sFile)
{
	return (HBITMAP)LoadImage(CApp::GetInstance()->m_hInstance, sFile.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
}

void CBitmap::Save(HBITMAP hBitmap, const string& sFile, HDC hDC)
{
	BITMAP oBitmap;
	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&oBitmap))
		return;

	short nColorBits = (short)(oBitmap.bmPlanes * oBitmap.bmBitsPixel); 
	if (nColorBits == 1) 
		nColorBits = 1; 
	else if (nColorBits <= 4) 
		nColorBits = 4; 
	else if (nColorBits <= 8) 
		nColorBits = 8; 
	else if (nColorBits <= 16) 
		nColorBits = 16; 
	else if (nColorBits <= 24) 
		nColorBits = 24; 
	else nColorBits = 32; 

	PBITMAPINFO pBitmapInfo;
	if (nColorBits != 24) 
		pBitmapInfo = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< nColorBits)); 
	else 
		pBitmapInfo = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 

	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	pBitmapInfo->bmiHeader.biWidth = oBitmap.bmWidth; 
	pBitmapInfo->bmiHeader.biHeight = oBitmap.bmHeight; 
	pBitmapInfo->bmiHeader.biPlanes = oBitmap.bmPlanes; 
	pBitmapInfo->bmiHeader.biBitCount = oBitmap.bmBitsPixel; 

	if (nColorBits < 24) 
		pBitmapInfo->bmiHeader.biClrUsed = (1 << nColorBits); 

	pBitmapInfo->bmiHeader.biCompression = BI_RGB; 
	pBitmapInfo->bmiHeader.biSizeImage = (pBitmapInfo->bmiHeader.biWidth + 7) / 8 * pBitmapInfo->bmiHeader.biHeight * nColorBits; 
	pBitmapInfo->bmiHeader.biClrImportant = 0; 

	PBITMAPINFOHEADER pBitmapInfoHeader = (PBITMAPINFOHEADER)pBitmapInfo; 
	LPBYTE pBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pBitmapInfoHeader->biSizeImage);

	if (!pBits) 
	{
		return;
	}

	if (!GetDIBits(hDC, HBITMAP(hBitmap), 0, pBitmapInfoHeader->biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS)) 
	{
		return;
	}

	BITMAPFILEHEADER oHeader;
	oHeader.bfType = 0x4d42;
	oHeader.bfSize = (DWORD)
		(
			sizeof(BITMAPFILEHEADER) +
			pBitmapInfoHeader->biSize +
			pBitmapInfoHeader->biClrUsed * sizeof(RGBQUAD) +
			pBitmapInfoHeader->biSizeImage
		);
	oHeader.bfReserved1 = 0;
	oHeader.bfReserved2 = 0;

	oHeader.bfOffBits =
		(DWORD) sizeof(BITMAPFILEHEADER) +
		pBitmapInfoHeader->biSize +
		pBitmapInfoHeader->biClrUsed * sizeof (RGBQUAD);

	HANDLE hFile = 
		CreateFile
		(
			sFile.c_str(), 
			GENERIC_READ | GENERIC_WRITE, 
			0, 
			0, 
			CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, 
			0
		); 

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD nWrittenBits; 
	if (!WriteFile(hFile, (LPVOID)&oHeader, sizeof(BITMAPFILEHEADER), &nWrittenBits, 0)) 
	{
		return;
	}

	if (!WriteFile
		(
			hFile, 
			pBitmapInfoHeader, 
			sizeof(BITMAPINFOHEADER) + 
			pBitmapInfoHeader->biClrUsed * sizeof (RGBQUAD), 
			&nWrittenBits, 
			0
		))
	{
		return;
	}

	if (!WriteFile(hFile, pBits, (int)pBitmapInfoHeader->biSizeImage, &nWrittenBits, 0))
	{
		return;
	}

	if (!CloseHandle(hFile))
	{
		return;
	}

	GlobalFree((HGLOBAL)pBits);
}

HBITMAP CBitmap::Create(HDC hDC, int nWidth, int nHeight)
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hMemDC, hBrush);

	HPEN hPen = CreatePen(PS_SOLID, 1, 0);
	HPEN hOldPen = (HPEN)SelectObject(hMemDC, hPen);

	Rectangle(hMemDC, -1, -1, nWidth + 2, nHeight + 2);

	SelectObject(hMemDC, hOldBrush);
	DeleteObject(hBrush);

	SelectObject(hMemDC, hOldPen);
	DeleteObject(hPen);

	SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hMemDC);

	return hBitmap;
}

