#ifndef WORKSPACE_WINDOW_H
#define WORKSPACE_WINDOW_H

#include "Headers/Headers.h"
#include "Window.h"
#include "DrawWindow.h"
#include "App/App.h"

class CWorkspaceWindow :
	public CWindow,
	public IStateChangedEventHandler
{
public:
	enum 
	{
		WMC_UPDATE = WM_USER + 1000
	};

	static BOOL RegisterClass();
	BOOL Create(CWindow* pParent);

	CWorkspaceWindow();
	virtual ~CWorkspaceWindow();

	void OnAppStateChanged(int nAction);

	void SetTool(CTool* pTool);
	void SetToolColors(COLORREF nForegroundColor, COLORREF nBackgroundColor);

protected:
	CDrawWindow* m_pDrawWindow;

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(CREATESTRUCT* pCreateStruct);
	virtual LRESULT OnDestroy();
	virtual LRESULT OnSize(UINT nAction, USHORT nNewWidth, USHORT nNewHeight);
	virtual LRESULT OnHScroll(WORD nAction);
	virtual LRESULT OnVScroll(WORD nAction);
	virtual LRESULT OnPaint();
	virtual LRESULT OnUpdate(int nAction);

private:
	static string m_sWindowClass;
	string m_sTitle;

	void OnScroll(int nBar, WORD nAction);
};

#endif