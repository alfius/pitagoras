#ifndef APP_H
#define APP_H

#include "Headers/Headers.h"
#include "Resource/resource.h"
#include "Windows/Window.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmdLine, int nCmdShow);

class IStateChangedEventHandler
{
public:
	enum
	{
		FILE_OPEN_CLOSE,
		FILE_MODIFIED
	};

	virtual void OnAppStateChanged(int nAction) = 0;
};

class CApp
{
public:
	HINSTANCE m_hInstance;
	string m_sCommandLine;
	int m_nCmdShow;

	CWindow* m_pMainWindow;

	HBITMAP m_hFile;
	string m_sFile;

	COLORREF m_nForegroundColor;
	COLORREF m_nBackgroundColor;

	COLORREF m_oCustomColors[16];

	static CApp* GetInstance();
	static void CreateInstance(HINSTANCE hInstance, string sCommandLine, int nCmdShow);
	static void TerminateInstance();

	int Run();

	string& LoadString(UINT nID, string* pSource);
	string& LoadFormatString(UINT nID, string* sSource, ...);
	HICON LoadIcon(UINT nID);
	HCURSOR LoadCursor(LPSTR szID);
	HBITMAP LoadBitmap(UINT nID, int nCX, int nCY);

	bool IsFileOpen();
	void OpenFile(const string& sFile);
	void CloseFile();
	void SaveFile(const string& sFile);
	void CreateFile();
	bool FileExists(const string& sFile);
	bool IsFileModified();
	void SetModified();

	void RegisterStateChangedEventHandler(IStateChangedEventHandler* pHandler);
	void UnregisterStateChangedEventHandler(IStateChangedEventHandler* pHandler);

private:
	CApp(HINSTANCE hInstance, string sCommandLine, int nCmdShow);
	virtual ~CApp();

	static const int MAX_LOADSTRING = 100;
	static CApp* m_pInstance;

	bool m_bFileModified;

	list<IStateChangedEventHandler*> m_oStateChangedEventHandlers;

	void RaiseStateChangedEvent(int nAction);
};

#endif
