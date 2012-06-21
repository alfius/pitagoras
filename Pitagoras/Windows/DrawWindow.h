#ifndef DRAW_WINDOW_H
#define DRAW_WINDOW_H

#include "Headers/Headers.h"
#include "Window.h"
#include "Tools/Tool.h"

class CDrawWindow :
	public CWindow
{
public:
	CDrawWindow();
	virtual ~CDrawWindow();

	static BOOL RegisterClass();
	BOOL Create(CWindow* pParent);
	void SetTool(CTool* pTool);
	void SetToolColors(COLORREF nForegroundColor, COLORREF nBackgroundColor);

protected:
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(CREATESTRUCT* pCreateStruct);
	virtual LRESULT OnPaint();
	virtual void DoPaint(HDC hDC, bool bIsPreview = true);
	virtual LRESULT OnSetCursor(HWND hWnd, UINT nHitTest, UINT nMsg);
	virtual LRESULT OnLButtonDown(UINT nModifier, short nX, short nY);
	virtual LRESULT OnLButtonUp(UINT nModifier, short nX, short nY);
	virtual LRESULT OnMouseMove(UINT nModifier, short nX, short nY);
	virtual LRESULT OnKeyDown(int nKey, int nCount, int nScanCode, bool bIsExtended);

private:
	static string m_sWindowClass;
	string m_sTitle;
	CTool* m_pCurrentTool;
	bool m_bUsingTool;

	short m_nLastMousePosX;
	short m_nLastMousePosY;

	HBITMAP TakeScreenshot();
};

#endif