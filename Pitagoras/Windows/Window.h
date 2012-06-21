#ifndef WINDOW_H
#define WINDOW_H

#include "Headers/Headers.h"

class CWindow
{
public:
	HWND m_hWnd;

	static LRESULT CALLBACK BaseWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	LRESULT SendMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL MoveWindow(int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	BOOL ShowWindow(int nCmdShow);
	BOOL UpdateWindow();
	BOOL GetClientRect(LPRECT pRect);
	void InvalidateRect(LPRECT pRect = 0, BOOL bErase = TRUE);
	HDC GetDC();
	int ReleaseDC(HDC hDC);

protected:
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
};

#endif