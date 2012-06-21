#ifndef CTRLCOLORPREVIEW
#define CTRLCOLORPREVIEW

#include "Headers/Headers.h"
#include "Windows/Window.h"
#include "Dialog.h"

class CCtrlColorPreview:
	public CWindow
{
public:
	COLORREF m_nColor;

	static BOOL RegisterClass();
	BOOL Create(CDialog* pParent);

protected:
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnPaint();

private:
	static string m_sWindowClass;
};

#endif