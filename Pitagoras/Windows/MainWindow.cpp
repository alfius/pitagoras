#include "MainWindow.h"
#include "App/App.h"
#include "Dialogs/DlgAbout.h"
#include "Dialogs/DlgColors.h"
#include "Exceptions/ExceptionInvalidOperation.h"
#include "Exceptions/ExceptionInvalidParameter.h"
#include "Tools/Pen.h"
#include "Tools/Rectangle.h"
#include "Tools/Ellipse.h"
#include "Tools/Fill.h"

BOOL CMainWindow::RegisterClass()
{
	if (m_sWindowClass.length() == 0)
		CApp::GetInstance()->LoadString(IDC_PITAGORAS, &m_sWindowClass);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= BaseWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= CApp::GetInstance()->m_hInstance;
	wcex.hIcon			= CApp::GetInstance()->LoadIcon(IDI_PITAGORAS);
	wcex.hIconSm		= CApp::GetInstance()->LoadIcon(IDI_SMALL);
	wcex.hCursor		= CApp::GetInstance()->LoadCursor(IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PITAGORAS);
	wcex.lpszClassName	= m_sWindowClass.c_str();

	return RegisterClassEx(&wcex);
}

BOOL CMainWindow::Create()
{
	CApp::GetInstance()->LoadString(IDS_APP_TITLE, &m_sTitle);

	CreateWindow
	(
		m_sWindowClass.c_str(),
		m_sTitle.c_str(), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		0, 
		CW_USEDEFAULT, 0, 
		0, 
		0, 
		CApp::GetInstance()->m_hInstance, 
		this
	);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(CApp::GetInstance()->m_nCmdShow);
	UpdateWindow();

	return TRUE;
}

CMainWindow::CMainWindow():
	m_pWorkspaceWindow(0),
	m_pToolBar(0)
{
}

CMainWindow::~CMainWindow()
{
	delete m_pWorkspaceWindow;
	delete m_pToolBar;
}

void CMainWindow::OnAppStateChanged(int nAction)
{
	SendMessage(WMC_UPDATE, nAction);
}

LRESULT CALLBACK CMainWindow::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (nMsg)
	{
	case WM_CREATE:
		return OnCreate((CREATESTRUCT*)lParam);

	case WM_DESTROY:
		OnDestroy();
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		return OnClose();

	case WM_SIZE:
		return OnSize(wParam, LOWORD(lParam), HIWORD(lParam));

	case WM_GETMINMAXINFO:
		return OnGetMinMaxInfo((MINMAXINFO*)lParam);

	case WMC_UPDATE:
		return OnUpdate(wParam);

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDM_NEW:
			return OnMenuNew();

		case IDM_OPEN:
			return OnMenuOpen();

		case IDM_SAVE:
			return OnMenuSave();

		case IDM_PEN:
			return OnMenuPen();

		case IDM_RECT:
			return OnMenuRect();

		case IDM_ELLIPSE:
			return OnMenuEllipse();

		case IDM_FILL:
			return OnMenuFill();

		case IDM_COLORS:
			return OnMenuColors();

		case IDM_ABOUT:
			return OnMenuAbout();

		case IDM_EXIT:
			return OnMenuQuit();

		default:
			return DefWindowProc(hWnd, nMsg, wParam, lParam);
		}
		break;

	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

LRESULT CMainWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	m_pToolBar = new CToolBar();
	if (!m_pToolBar->Create(this))
	{
		return FALSE;
	}

	UpdateToolBar();

	m_pWorkspaceWindow = new CWorkspaceWindow();
	if (!m_pWorkspaceWindow->Create(this))
	{
		return FALSE;
	}

	CApp::GetInstance()->RegisterStateChangedEventHandler(this);

	return TRUE;
}

LRESULT CMainWindow::OnDestroy()
{
	CApp::GetInstance()->UnregisterStateChangedEventHandler(this);

	return m_pToolBar->Destroy();
}

LRESULT CMainWindow::OnClose()
{
	if (EnsureFileClose())
		return DestroyWindow(m_hWnd);

	return TRUE;
}

LRESULT CMainWindow::OnSize(UINT nAction, USHORT nNewWidth, USHORT nNewHeight)
{
	RECT oToolBarRect;
	m_pToolBar->GetClientRect(&oToolBarRect);

	const int nMargin = 0;
	int nToolBarHeight = oToolBarRect.bottom;

	RECT oRect;
	GetClientRect(&oRect);

	m_pWorkspaceWindow->MoveWindow(nMargin, nMargin + nToolBarHeight, oRect.right - (2 * nMargin), oRect.bottom - ((2 * nMargin) + nToolBarHeight));

	return TRUE;
}

LRESULT CMainWindow::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	pMinMaxInfo->ptMinTrackSize.x = 300;
	pMinMaxInfo->ptMinTrackSize.y = 200;

	return TRUE;
}
LRESULT CMainWindow::OnUpdate(int nAction)
{
	switch (nAction)
	{
	case IStateChangedEventHandler::FILE_MODIFIED:
	case IStateChangedEventHandler::FILE_OPEN_CLOSE:
		UpdateToolBar();
		return TRUE;
	}

	return TRUE;
}

LRESULT CMainWindow::OnMenuNew()
{
	if (!EnsureFileClose())
		return TRUE;

	CApp::GetInstance()->CreateFile();

	return TRUE;
}

LRESULT CMainWindow::OnMenuOpen()
{
	if (!EnsureFileClose())
		return TRUE;

	char szFileName[2048];
	szFileName[0] = 0;

	OPENFILENAME oFile;
	memset(&oFile, 0, sizeof(OPENFILENAME));
	oFile.lStructSize = sizeof(OPENFILENAME);
	oFile.hwndOwner = m_hWnd;
	oFile.hInstance = CApp::GetInstance()->m_hInstance;
	oFile.lpstrFile = szFileName;
	oFile.nMaxFile = 2048;
	oFile.lpstrFilter = "Mapas de bits (*.bmp)\0*.bmp\0\0\0";
	oFile.lpstrDefExt = ".bmp";
	if (!GetOpenFileName(&oFile))
		return TRUE;

	try
	{
		CApp::GetInstance()->OpenFile(szFileName);
	}
	catch (CExceptionInvalidParameter&)
	{
		string sError;
		CApp::GetInstance()->LoadString(IDS_EXCEPTION_INVALID_FILE, &sError);
		MessageBox(m_hWnd, sError.c_str(), m_sTitle.c_str(), MB_ICONERROR);

		return TRUE;
	}

	return TRUE;

}

LRESULT CMainWindow::OnMenuSave()
{
	Save();

	return TRUE;
}

LRESULT CMainWindow::OnMenuPen()
{
	if (m_pWorkspaceWindow)
		m_pWorkspaceWindow->SetTool
		(
			new CPen
			(
				CApp::GetInstance()->m_nForegroundColor, 
				CApp::GetInstance()->m_nBackgroundColor
			)
		);

	return TRUE;
}

LRESULT CMainWindow::OnMenuRect()
{
	if (m_pWorkspaceWindow)
		m_pWorkspaceWindow->SetTool
		(
			new CRectangle
			(
				CApp::GetInstance()->m_nForegroundColor, 
				CApp::GetInstance()->m_nBackgroundColor
			)
		);

	return TRUE;
}

LRESULT CMainWindow::OnMenuEllipse()
{
	if (m_pWorkspaceWindow)
		m_pWorkspaceWindow->SetTool
		(
			new CEllipse
			(
				CApp::GetInstance()->m_nForegroundColor, 
				CApp::GetInstance()->m_nBackgroundColor
			)
		);

	return TRUE;
}

LRESULT CMainWindow::OnMenuFill()
{
	if (m_pWorkspaceWindow)
		m_pWorkspaceWindow->SetTool
		(
			new CFill
			(
				CApp::GetInstance()->m_nForegroundColor, 
				CApp::GetInstance()->m_nBackgroundColor
			)
		);

	return TRUE;
}

LRESULT CMainWindow::OnMenuColors()
{
	CDlgColors oDlg(this);
	oDlg.m_nForegroundColor = CApp::GetInstance()->m_nForegroundColor;
	oDlg.m_nBackgroundColor = CApp::GetInstance()->m_nBackgroundColor;
	if (IDOK == oDlg.DoModal())
	{
		CApp::GetInstance()->m_nForegroundColor = oDlg.m_nForegroundColor;
		CApp::GetInstance()->m_nBackgroundColor = oDlg.m_nBackgroundColor;

		if (m_pWorkspaceWindow)
		{
			m_pWorkspaceWindow->SetToolColors
			(
				CApp::GetInstance()->m_nForegroundColor, 
				CApp::GetInstance()->m_nBackgroundColor
			);
		}

		m_pToolBar->Update();
	}

	return TRUE;
}

LRESULT CMainWindow::OnMenuAbout()
{
	CDlgAbout oDlg(this);
	oDlg.DoModal();

	return TRUE;
}

LRESULT CMainWindow::OnMenuQuit()
{
	DestroyWindow(m_hWnd);

	return TRUE;
}

string CMainWindow::m_sWindowClass;

bool CMainWindow::Save()
{
	char szFileName[2048];
	strncpy_s(szFileName, 2048, CApp::GetInstance()->m_sFile.c_str(), CApp::GetInstance()->m_sFile.length());

	OPENFILENAME oFile;
	memset(&oFile, 0, sizeof(OPENFILENAME));
	oFile.lStructSize = sizeof(OPENFILENAME);
	oFile.hwndOwner = m_hWnd;
	oFile.hInstance = CApp::GetInstance()->m_hInstance;
	oFile.lpstrFile = szFileName;
	oFile.nMaxFile = 2048;
	oFile.lpstrFilter = "Mapas de bits (*.bmp)\0*.bmp\0\0\0";
	oFile.lpstrDefExt = ".bmp";
	if (!GetSaveFileName(&oFile))
		return false;

	if (CApp::GetInstance()->FileExists(szFileName))
	{
		string sAskReplace;
		CApp::GetInstance()->LoadFormatString(IDS_ASK_REPLACE_FILE, &sAskReplace, szFileName);
		switch (MessageBox(m_hWnd, sAskReplace.c_str(), m_sTitle.c_str(), MB_ICONQUESTION | MB_YESNOCANCEL))
		{
		case IDNO:
		case IDCANCEL:
			return false;
		}
	}

	try
	{
		CApp::GetInstance()->SaveFile(szFileName);
	}
	catch (CExceptionInvalidParameter&)
	{
		string sError;
		CApp::GetInstance()->LoadString(IDS_EXCEPTION_INVALID_FILE, &sError);
		MessageBox(m_hWnd, sError.c_str(), m_sTitle.c_str(), MB_ICONERROR);

		return false;
	}

	return true;
}

bool CMainWindow::EnsureFileClose()
{
	if (CApp::GetInstance()->IsFileOpen() && CApp::GetInstance()->IsFileModified())
	{
		string sAskSaveFile;
		CApp::GetInstance()->LoadFormatString(IDS_ASK_SAVE_FILE, &sAskSaveFile, CApp::GetInstance()->m_sFile.c_str());
		switch (MessageBox(m_hWnd, sAskSaveFile.c_str(), m_sTitle.c_str(), MB_YESNOCANCEL))
		{
		case IDYES:
			if (!Save())
				return false;

		case IDNO:
			CApp::GetInstance()->CloseFile();

			return true;

		case IDCANCEL:
			return false;
		}
	}
	else if (CApp::GetInstance()->IsFileOpen())
	{
		CApp::GetInstance()->CloseFile();
		return true;
	}

	return !(CApp::GetInstance()->IsFileOpen());
}

void CMainWindow::UpdateToolBar()
{
	MENUITEMINFO oMenuItemInfoDisabled;
	oMenuItemInfoDisabled.cbSize = sizeof(MENUITEMINFO);
	oMenuItemInfoDisabled.fState = MFS_DISABLED;
	oMenuItemInfoDisabled.fMask = MIIM_STATE;

	MENUITEMINFO oMenuItemInfoEnabled;
	oMenuItemInfoEnabled.cbSize = sizeof(MENUITEMINFO);
	oMenuItemInfoEnabled.fState = MFS_ENABLED;
	oMenuItemInfoEnabled.fMask = MIIM_STATE;

	HMENU hMenu = GetMenu(m_hWnd);
	HMENU hMenuFile = GetSubMenu(hMenu, 0);
	HMENU hMenuTools = GetSubMenu(hMenu, 1);

	if (!CApp::GetInstance()->m_hFile)
	{
		m_pToolBar->EnableButton(IDM_NEW);
		m_pToolBar->EnableButton(IDM_OPEN);
		m_pToolBar->EnableButton(IDM_SAVE, 0);
		m_pToolBar->EnableButton(IDM_PEN, 0);
		m_pToolBar->EnableButton(IDM_RECT, 0);
		m_pToolBar->EnableButton(IDM_ELLIPSE, 0);
		m_pToolBar->EnableButton(IDM_FILL, 0);
		m_pToolBar->EnableButton(IDM_COLORS, 0);

		SetMenuItemInfo(hMenuFile, IDM_NEW, FALSE, &oMenuItemInfoEnabled);
		SetMenuItemInfo(hMenuFile, IDM_OPEN, FALSE, &oMenuItemInfoEnabled);
		SetMenuItemInfo(hMenuFile, IDM_SAVE, FALSE, &oMenuItemInfoDisabled);

		SetMenuItemInfo(hMenuTools, IDM_PEN, FALSE, &oMenuItemInfoDisabled);
		SetMenuItemInfo(hMenuTools, IDM_RECT, FALSE, &oMenuItemInfoDisabled);
		SetMenuItemInfo(hMenuTools, IDM_ELLIPSE, FALSE, &oMenuItemInfoDisabled);
		SetMenuItemInfo(hMenuTools, IDM_FILL, FALSE, &oMenuItemInfoDisabled);
		SetMenuItemInfo(hMenuTools, IDM_COLORS, FALSE, &oMenuItemInfoDisabled);
	}
	else
	{
		m_pToolBar->EnableButton(IDM_NEW);
		m_pToolBar->EnableButton(IDM_OPEN);
		m_pToolBar->EnableButton(IDM_SAVE, CApp::GetInstance()->IsFileModified());
		m_pToolBar->EnableButton(IDM_PEN);
		m_pToolBar->EnableButton(IDM_RECT);
		m_pToolBar->EnableButton(IDM_ELLIPSE);
		m_pToolBar->EnableButton(IDM_FILL);
		m_pToolBar->EnableButton(IDM_COLORS);

		SetMenuItemInfo(hMenuFile, IDM_NEW, FALSE, &oMenuItemInfoEnabled);
		SetMenuItemInfo(hMenuFile, IDM_OPEN, FALSE, &oMenuItemInfoEnabled);
		SetMenuItemInfo
		(
			hMenuFile, 
			IDM_SAVE, 
			FALSE, 
			CApp::GetInstance()->IsFileModified() ? &oMenuItemInfoEnabled : &oMenuItemInfoDisabled
		);

		SetMenuItemInfo(hMenuTools, IDM_PEN, FALSE, &oMenuItemInfoEnabled);
		SetMenuItemInfo(hMenuTools, IDM_RECT, FALSE, &oMenuItemInfoEnabled);
		SetMenuItemInfo(hMenuTools, IDM_ELLIPSE, FALSE, &oMenuItemInfoEnabled);
		SetMenuItemInfo(hMenuTools, IDM_FILL, FALSE, &oMenuItemInfoEnabled);
		SetMenuItemInfo(hMenuTools, IDM_COLORS, FALSE, &oMenuItemInfoEnabled);
	}
}
