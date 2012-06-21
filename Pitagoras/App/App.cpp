#include "App.h"
#include "Bitmap.h"
#include "Windows/MainWindow.h"
#include "Windows/DrawWindow.h"
#include "Dialogs/CtrlColorPreview.h"
#include "Exceptions/ExceptionInvalidOperation.h"
#include "Exceptions/ExceptionInvalidParameter.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	int nResult = 0;
	
	try
	{
		CApp::CreateInstance(hInstance, lpCmdLine, nCmdShow);
		nResult = CApp::GetInstance()->Run();
		CApp::TerminateInstance();
	}
	catch (bad_alloc&)
	{
		nResult = -1;
	}

	return nResult;
}

CApp::CApp(HINSTANCE hInstance, string sCommandLine, int nCmdShow):
	m_hInstance(hInstance), 
	m_sCommandLine(sCommandLine), 
	m_nCmdShow(nCmdShow),
	m_hFile(0),
	m_nForegroundColor(RGB(91, 125, 91)),
	m_nBackgroundColor(RGB(127, 232, 127)),
	m_bFileModified(false)
{
	for (int i = 0; i < 16; i++)
	{
		m_oCustomColors[i] = 0xffffff;
	}
}

CApp::~CApp()
{
	CloseFile();
}

int CApp::Run()
{
	// Window classes
	if (!CMainWindow::RegisterClass())
	{
		return FALSE;
	}

	if (!CWorkspaceWindow::RegisterClass())
	{
		return FALSE;
	}

	if (!CDrawWindow::RegisterClass())
	{
		return FALSE;
	}

	if (!CCtrlColorPreview::RegisterClass())
	{
		return FALSE;
	}

	// Libraries
	InitCommonControls();

	CMainWindow oWnd;
	m_pMainWindow = &oWnd;
	if (!oWnd.Create())
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_PITAGORAS));

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

string& CApp::LoadString(UINT nID, string* pSource)
{
	char szBuffer[MAX_LOADSTRING];
	::LoadString(m_hInstance, nID, szBuffer, MAX_LOADSTRING);

	*pSource = szBuffer;
	return *pSource;
}

string& CApp::LoadFormatString(UINT nID, string* sSource, ...)
{
	char szFormatBuffer[MAX_LOADSTRING];
	::LoadString(m_hInstance, nID, szFormatBuffer, MAX_LOADSTRING);

	va_list oArgs;
	va_start(oArgs, sSource);

	char szResultBuffer[MAX_LOADSTRING];
	szResultBuffer[0] = 0;
	vsnprintf_s(szResultBuffer, MAX_LOADSTRING, MAX_LOADSTRING, szFormatBuffer, oArgs);

	va_end(oArgs);

	*sSource = szResultBuffer;
	return *sSource;
}

HICON CApp::LoadIcon(UINT nID)
{
	return ::LoadIcon(m_hInstance, MAKEINTRESOURCE(nID));
}

HCURSOR CApp::LoadCursor(LPSTR szID)
{
	HINSTANCE hInstance = 0;
	switch ((UINT)szID)
	{
	case IDC_ARROW:
	case IDC_IBEAM:
	case IDC_WAIT:
	case IDC_CROSS:
	case IDC_UPARROW:
	case IDC_SIZE:
	case IDC_ICON:
	case IDC_SIZENWSE:
	case IDC_SIZENESW:
	case IDC_SIZEWE:
	case IDC_SIZENS:
	case IDC_SIZEALL:
	case IDC_NO:
	case IDC_HAND:
	case IDC_APPSTARTING:
	case IDC_HELP:
		hInstance = 0;
		break;
	default:
		hInstance = m_hInstance;
	}

	return ::LoadCursor(hInstance, szID);
}

HBITMAP CApp::LoadBitmap(UINT nID, int nCX, int nCY)
{
	return (HBITMAP)::LoadImage(m_hInstance, MAKEINTRESOURCE(nID), IMAGE_BITMAP, nCX, nCY, LR_DEFAULTCOLOR);
}

bool CApp::IsFileOpen()
{
	return m_hFile != 0;
}

void CApp::OpenFile(const string& sFile)
{
	if (IsFileOpen())
		throw CExceptionInvalidOperation();

	m_hFile = CBitmap::Load(sFile.c_str());

	if (!m_hFile)
		throw CExceptionInvalidParameter("sFile");

	m_sFile = sFile.substr(sFile.rfind("\\") + 1);
	m_bFileModified = false;

	RaiseStateChangedEvent(IStateChangedEventHandler::FILE_OPEN_CLOSE);
}

void CApp::CloseFile()
{
	if (m_hFile)
	{
		DeleteObject(m_hFile);
		m_hFile = 0;

		RaiseStateChangedEvent(IStateChangedEventHandler::FILE_OPEN_CLOSE);
	}
}

void CApp::SaveFile(const string& sFile)
{
	CBitmap::Save(m_hFile, sFile, m_pMainWindow->GetDC());

	m_bFileModified = false;

	RaiseStateChangedEvent(IStateChangedEventHandler::FILE_MODIFIED);
}

void CApp::CreateFile()
{
	if (IsFileOpen())
		throw CExceptionInvalidOperation();

	m_hFile = CBitmap::Create(m_pMainWindow->GetDC(), 300, 300);

	m_sFile = "Nuevo";
	SetModified();

	RaiseStateChangedEvent(IStateChangedEventHandler::FILE_OPEN_CLOSE);
}

bool CApp::FileExists(const string& sFile)
{
	WIN32_FIND_DATA oData;
	HANDLE hFound = FindFirstFile(sFile.c_str(), &oData);
	if (hFound != INVALID_HANDLE_VALUE)
	{
		FindClose(hFound);

		return true;
	}

	return false;
}

bool CApp::IsFileModified()
{
	return m_bFileModified;
}

void CApp::SetModified()
{
	m_bFileModified = true;

	RaiseStateChangedEvent(IStateChangedEventHandler::FILE_MODIFIED);
}

void CApp::RegisterStateChangedEventHandler(IStateChangedEventHandler* pHandler)
{
	m_oStateChangedEventHandlers.push_back(pHandler);
}

void CApp::UnregisterStateChangedEventHandler(IStateChangedEventHandler* pHandler)
{
	m_oStateChangedEventHandlers.remove(pHandler);
}

CApp* CApp::m_pInstance = 0;

CApp* CApp::GetInstance()
{
	return m_pInstance;
}

void CApp::CreateInstance(HINSTANCE hInstance, string sCommandLine, int nCmdShow)
{
	m_pInstance = new CApp(hInstance, sCommandLine, nCmdShow);
}

void CApp::TerminateInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
}

void CApp::RaiseStateChangedEvent(int nAction)
{
	for 
	(
		list<IStateChangedEventHandler*>::iterator itHandler = 
			m_oStateChangedEventHandlers.begin(); 
		itHandler != m_oStateChangedEventHandlers.end(); 
		itHandler++)
	{
		(*itHandler)->OnAppStateChanged(nAction);
	}
}
