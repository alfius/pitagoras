#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "Headers/Headers.h"
#include "Window.h"
#include "WorkspaceWindow.h"
#include "ToolBar.h"
#include "App/App.h"

class CMainWindow :
	public CWindow,
	public IStateChangedEventHandler
{
public:
	enum 
	{
		WMC_UPDATE = WM_USER + 1000
	};

	static BOOL RegisterClass();
	BOOL Create();

	CMainWindow();
	virtual ~CMainWindow();

	virtual void OnAppStateChanged(int nAction);

protected:
	CToolBar* m_pToolBar;
	CWorkspaceWindow* m_pWorkspaceWindow;

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(CREATESTRUCT* pCreateStruct);
	virtual LRESULT OnDestroy();
	virtual LRESULT OnClose();
	virtual LRESULT OnSize(UINT nAction, USHORT nNewWidth, USHORT nNewHeight);
	virtual LRESULT OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo);
	virtual LRESULT OnUpdate(int nAction);
	virtual LRESULT OnMenuNew();
	virtual LRESULT OnMenuOpen();
	virtual LRESULT OnMenuSave();
	virtual LRESULT OnMenuPen();
	virtual LRESULT OnMenuRect();
	virtual LRESULT OnMenuEllipse();
	virtual LRESULT OnMenuFill();
	virtual LRESULT OnMenuColors();
	virtual LRESULT OnMenuAbout();
	virtual LRESULT OnMenuQuit();

private:
	static string m_sWindowClass;
	string m_sTitle;

	bool Save();
	bool EnsureFileClose();
	void UpdateToolBar();
};

#endif