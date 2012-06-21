#include "WorkspaceWindow.h"
#include "App/App.h"

BOOL CWorkspaceWindow::RegisterClass()
{
	if (m_sWindowClass.length() == 0)
		CApp::GetInstance()->LoadString(IDS_WORKSPACE_WINDOW, &m_sWindowClass);

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

BOOL CWorkspaceWindow::Create(CWindow* pParent)
{
	CApp::GetInstance()->LoadString(IDS_WORKSPACE_WINDOW_TITLE, &m_sTitle);

	CreateWindow
	(
		m_sWindowClass.c_str(),
		m_sTitle.c_str(), 
		WS_CHILD | WS_HSCROLL | WS_VSCROLL,
		CW_USEDEFAULT, 
		0, 
		CW_USEDEFAULT, 0, 
		pParent->m_hWnd, 
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

CWorkspaceWindow::CWorkspaceWindow():
	m_pDrawWindow(0)
{
}

CWorkspaceWindow::~CWorkspaceWindow()
{
	delete m_pDrawWindow;
}

void CWorkspaceWindow::OnAppStateChanged(int nAction)
{
	SendMessage(WMC_UPDATE, nAction);
}

void CWorkspaceWindow::SetTool(CTool* pTool)
{
	if (m_pDrawWindow)
		m_pDrawWindow->SetTool(pTool);
}

void CWorkspaceWindow::SetToolColors(COLORREF nForegroundColor, COLORREF nBackgroundColor)
{
	if (m_pDrawWindow)
		m_pDrawWindow->SetToolColors(nForegroundColor, nBackgroundColor);
}

LRESULT CALLBACK CWorkspaceWindow::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_CREATE:
		return OnCreate((CREATESTRUCT*)lParam);

	case WM_DESTROY:
		return OnDestroy();

	case WM_SIZE:
		return OnSize(wParam, LOWORD(lParam), HIWORD(lParam));

	case WM_HSCROLL:
		return OnHScroll(LOWORD(wParam));

	case WM_VSCROLL:
		return OnVScroll(LOWORD(wParam));

	case WM_PAINT:
		return OnPaint();

	case WMC_UPDATE:
		return OnUpdate(wParam);

	default:
		return CWindow::WndProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

LRESULT CWorkspaceWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	m_pDrawWindow = new CDrawWindow();
	if (!m_pDrawWindow->Create(this))
	{
		return FALSE;
	}

	CApp::GetInstance()->RegisterStateChangedEventHandler(this);

	return TRUE;
}

LRESULT CWorkspaceWindow::OnDestroy()
{
	CApp::GetInstance()->UnregisterStateChangedEventHandler(this);
	return TRUE;
}

LRESULT CWorkspaceWindow::OnSize(UINT nAction, USHORT nNewWidth, USHORT nNewHeight)
{
	if (!CApp::GetInstance()->m_hFile)
	{
		m_pDrawWindow->MoveWindow(0, 0, 0, 0);

		SCROLLINFO oScrollInfo;
		oScrollInfo.cbSize = sizeof(SCROLLINFO);
		oScrollInfo.nMin = 0;
		oScrollInfo.nMax = 0;
		oScrollInfo.nPage = 0;
		oScrollInfo.nPos = 0;
		oScrollInfo.nTrackPos = 0;
		oScrollInfo.fMask = SIF_ALL;
		SetScrollInfo(m_hWnd, SB_HORZ, &oScrollInfo, TRUE);
		SetScrollInfo(m_hWnd, SB_VERT, &oScrollInfo, TRUE);
	}
	else
	{
		BITMAP oBitmapInfo;
		GetObject(CApp::GetInstance()->m_hFile, sizeof(BITMAP), &oBitmapInfo);

		m_pDrawWindow->MoveWindow
		(
			0, 
			0, 
			oBitmapInfo.bmWidth, 
			oBitmapInfo.bmHeight
		);

		RECT oRect;
		GetClientRect(&oRect);

		SCROLLINFO oScrollInfo;
		oScrollInfo.cbSize = sizeof(SCROLLINFO);
		oScrollInfo.nMin = 0;
		oScrollInfo.nMax = oBitmapInfo.bmWidth;
		oScrollInfo.nPage = oRect.right;
		oScrollInfo.nPos = 0;
		oScrollInfo.nTrackPos = 0;
		oScrollInfo.fMask = SIF_ALL;
		SetScrollInfo(m_hWnd, SB_HORZ, &oScrollInfo, TRUE);
		oScrollInfo.nPage = oRect.bottom;
		oScrollInfo.nMax = oBitmapInfo.bmHeight;
		SetScrollInfo(m_hWnd, SB_VERT, &oScrollInfo, TRUE);
	}

	return TRUE;
}

LRESULT CWorkspaceWindow::OnHScroll(WORD nAction)
{
	OnScroll(SB_HORZ, nAction);
	return TRUE;
}

LRESULT CWorkspaceWindow::OnVScroll(WORD nAction)
{
	OnScroll(SB_VERT, nAction);
	return TRUE;
}

LRESULT CWorkspaceWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(m_hWnd, &ps);

	RECT oRect;
    GetClientRect(&oRect);

    HDC hDCDoubleBuffering = CreateCompatibleDC(hDC);
    HBITMAP bmDoubleBuffering = CreateCompatibleBitmap(hDC, oRect.right, oRect.bottom);
    SelectObject(hDCDoubleBuffering, bmDoubleBuffering);

	SetBkMode(hDCDoubleBuffering, TRANSPARENT);

    {
		BYTE nBGColorR = 155;
		BYTE nBGColorG = 255;
		BYTE nBGColorB = 255;
        const COLORREF color1 = RGB(
            nBGColorR + 100 <= 255 ? nBGColorR + 100 : 255,
            nBGColorG + 100 <= 255 ? nBGColorG + 100 : 255,
            nBGColorB + 100 <= 255 ? nBGColorB + 100 : 255);
        const COLORREF color2 = RGB(nBGColorR, nBGColorG, nBGColorB);

        MoveToEx(hDCDoubleBuffering, 0, 0, 0);
        for (int i = 0; i < oRect.bottom; i++)
        {
            int deltaR = (int)(((double)i * (double)(GetRValue(color2) - GetRValue(color1))) / (double)oRect.bottom);
            int deltaG = (int)(((double)i * (double)(GetGValue(color2) - GetGValue(color1))) / (double)oRect.bottom);
            int deltaB = (int)(((double)i * (double)(GetBValue(color2) - GetBValue(color1))) / (double)oRect.bottom);

            COLORREF currentColor = RGB(
                GetRValue(color1)+deltaR,
                GetGValue(color1)+deltaG,
                GetBValue(color1)+deltaB);

            HPEN pen = CreatePen(PS_SOLID, 1, currentColor);
            HPEN oldPen = (HPEN)SelectObject(hDCDoubleBuffering, pen);

            LineTo(hDCDoubleBuffering, oRect.right, i);
            MoveToEx(hDCDoubleBuffering, 0, i+1, 0);

            DeleteObject((HPEN)SelectObject(hDCDoubleBuffering, oldPen));
        }
    }
	
	BitBlt(hDC, 0, 0, oRect.right, oRect.bottom, hDCDoubleBuffering, 0, 0, SRCCOPY);
    DeleteDC(hDCDoubleBuffering);
    DeleteObject(bmDoubleBuffering);

	EndPaint(m_hWnd, &ps);

	return TRUE;
}

LRESULT CWorkspaceWindow::OnUpdate(int nAction)
{
	switch (nAction)
	{
	case IStateChangedEventHandler::FILE_OPEN_CLOSE:
		OnSize(0, 0, 0);
		m_pDrawWindow->InvalidateRect();
		return TRUE;

	case IStateChangedEventHandler::FILE_MODIFIED:
		return TRUE;
	}

	return TRUE;
}

string CWorkspaceWindow::m_sWindowClass;

void CWorkspaceWindow::OnScroll(int nBar, WORD nAction)
{
	SCROLLINFO oScrollInfo;
	oScrollInfo.cbSize = sizeof(SCROLLINFO);
    oScrollInfo.fMask = SIF_ALL;
    GetScrollInfo(m_hWnd, nBar, &oScrollInfo);

	int nPrevPos = oScrollInfo.nPos;
	switch (LOWORD(nAction))
	{
	case SB_LEFT:
		oScrollInfo.nPos = oScrollInfo.nMin;
		break;

	case SB_RIGHT:
		oScrollInfo.nPos = oScrollInfo.nMax;
		break;

	case SB_LINELEFT: 
		oScrollInfo.nPos -= 1;
		break;

	case SB_LINERIGHT: 
		oScrollInfo.nPos += 1;
		break;
	  
	case SB_PAGELEFT:
		oScrollInfo.nPos -= oScrollInfo.nPage;
		break;
	
	case SB_PAGERIGHT:
		oScrollInfo.nPos += oScrollInfo.nPage;
		break;
	  
	case SB_THUMBTRACK: 
		oScrollInfo.nPos = oScrollInfo.nTrackPos;
		break;
	}
	
	oScrollInfo.fMask = SIF_POS;
	SetScrollInfo (m_hWnd, nBar, &oScrollInfo, TRUE);
	GetScrollInfo (m_hWnd, nBar, &oScrollInfo);

	if (oScrollInfo.nPos != nPrevPos)
	{
		ScrollWindow
		(
			m_hWnd, 
			nBar == SB_HORZ ? nPrevPos - oScrollInfo.nPos : 0, 
			nBar == SB_VERT ? nPrevPos - oScrollInfo.nPos : 0,
			0, 
			0
		);
	}
}
