#include "DrawWindow.h"
#include "App/App.h"

CDrawWindow::CDrawWindow():
	m_pCurrentTool(0),
	m_bUsingTool(false)
{
}

CDrawWindow::~CDrawWindow()
{
	delete m_pCurrentTool;
}

BOOL CDrawWindow::RegisterClass()
{
	if (m_sWindowClass.length() == 0)
		CApp::GetInstance()->LoadString(IDS_DRAW_WINDOW, &m_sWindowClass);

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
	wcex.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PITAGORAS);
	wcex.lpszClassName	= m_sWindowClass.c_str();

	return RegisterClassEx(&wcex);
}

BOOL CDrawWindow::Create(CWindow* pParent)
{
	CApp::GetInstance()->LoadString(IDS_DRAW_WINDOW_TITLE, &m_sTitle);

	CreateWindow
	(
		m_sWindowClass.c_str(),
		m_sTitle.c_str(), 
		WS_CHILD,
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

	ShowWindow(SW_SHOW);
	UpdateWindow();

	return TRUE;
}

void CDrawWindow::SetTool(CTool* pTool)
{
	delete m_pCurrentTool;

	m_pCurrentTool = pTool;
}

void CDrawWindow::SetToolColors(COLORREF nForegroundColor, COLORREF nBackgroundColor)
{
	if (m_pCurrentTool)
	{
		m_pCurrentTool->m_nForegroundColor = nForegroundColor;
		m_pCurrentTool->m_nBackgroundColor = nBackgroundColor;
	}
}

LRESULT CALLBACK CDrawWindow::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_CREATE:
		return OnCreate((CREATESTRUCT*)lParam);

	case WM_PAINT:
		return OnPaint();

	case WM_SETCURSOR:
		return OnSetCursor((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));

	case WM_LBUTTONDOWN:
		return OnLButtonDown((UINT)wParam, LOWORD(lParam), HIWORD(lParam));

	case WM_LBUTTONUP:
		return OnLButtonUp((UINT)wParam, LOWORD(lParam), HIWORD(lParam));

	case WM_MOUSEMOVE:
		return OnMouseMove((UINT)wParam, LOWORD(lParam), HIWORD(lParam));

	case WM_KEYDOWN:
		return OnKeyDown(wParam, lParam & 0xffff, (lParam & 0xff0000) >> 16, (lParam & 0x1000000) > 0);

	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

LRESULT CDrawWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	return TRUE;
}

LRESULT CDrawWindow::OnPaint()
{
	if (!CApp::GetInstance()->m_hFile)
		return TRUE;

	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(m_hWnd, &ps);

	DoPaint(hDC);

	EndPaint(m_hWnd, &ps);

	return TRUE;
}

void CDrawWindow::DoPaint(HDC hDC, bool bIsPreview)
{
	RECT oRect;
    GetClientRect(&oRect);


	// hDCDoubleBuffering

    HDC hDCDoubleBuffering = CreateCompatibleDC(hDC);
    HBITMAP bmDoubleBuffering = CreateCompatibleBitmap(hDC, oRect.right, oRect.bottom);
    SelectObject(hDCDoubleBuffering, bmDoubleBuffering);

	SetBkMode(hDCDoubleBuffering, TRANSPARENT);


	// Bitmap

	HDC hDCBitmap = CreateCompatibleDC(hDCDoubleBuffering);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCBitmap, CApp::GetInstance()->m_hFile);

	BITMAP oBitmapInfo;
	GetObject(CApp::GetInstance()->m_hFile, sizeof(BITMAP), &oBitmapInfo);

	BitBlt(hDCDoubleBuffering, 0, 0, oBitmapInfo.bmWidth, oBitmapInfo.bmHeight, hDCBitmap, 0, 0, SRCCOPY);

	SelectObject(hDCBitmap, hOldBitmap);
    DeleteDC(hDCBitmap);


	// m_pCurrentTool

	if (m_bUsingTool && m_pCurrentTool && (!bIsPreview || m_pCurrentTool->PreviewTool()))
	{
		m_pCurrentTool->OnPaint(hDCDoubleBuffering);
	}

	
	// hDCDoubleBuffering -> hDC

	BitBlt(hDC, 0, 0, oRect.right, oRect.bottom, hDCDoubleBuffering, 0, 0, SRCCOPY);
    DeleteDC(hDCDoubleBuffering);
    DeleteObject(bmDoubleBuffering);
}

LRESULT CDrawWindow::OnSetCursor(HWND hWnd, UINT nHitTest, UINT nMsg)
{
	if (m_pCurrentTool)
	{
		SetCursor(CApp::GetInstance()->LoadCursor(m_pCurrentTool->GetToolCursor()));
		return TRUE;
	}

	return FALSE;
}

LRESULT CDrawWindow::OnLButtonDown(UINT nModifier, short nX, short nY)
{
	if (m_pCurrentTool)
	{
		m_nLastMousePosX = nX;
		m_nLastMousePosY = nY;

		SetFocus(m_hWnd);

		SetCapture(m_hWnd);
		m_pCurrentTool->OnToolStart(nModifier, nX, nY);
		m_bUsingTool = true;

		InvalidateRect(0, FALSE);
	}

	return TRUE;
}

LRESULT CDrawWindow::OnLButtonUp(UINT nModifier, short nX, short nY)
{
	if (m_pCurrentTool && m_bUsingTool)
	{
		m_nLastMousePosX = nX;
		m_nLastMousePosY = nY;

		HBITMAP hNewBitmap = TakeScreenshot();
		DeleteObject(CApp::GetInstance()->m_hFile);
		CApp::GetInstance()->m_hFile = hNewBitmap;

		CApp::GetInstance()->SetModified();

		m_pCurrentTool->OnToolEnd(nModifier, nX, nY);
		ReleaseCapture();
		m_bUsingTool = false;

		InvalidateRect(0, FALSE);
	}

	return TRUE;
}

LRESULT CDrawWindow::OnMouseMove(UINT nModifier, short nX, short nY)
{
	if (m_pCurrentTool && m_bUsingTool)
	{
		m_nLastMousePosX = nX;
		m_nLastMousePosY = nY;

		m_pCurrentTool->OnToolUse(nModifier, nX, nY);

		InvalidateRect(0, FALSE);
	}

	return TRUE;
}

LRESULT CDrawWindow::OnKeyDown(int nKey, int nCount, int nScanCode, bool bIsExtended)
{
	if (nKey == VK_ESCAPE && m_pCurrentTool && m_bUsingTool)
	{
		m_pCurrentTool->OnToolEnd(0, m_nLastMousePosX, m_nLastMousePosY);
		ReleaseCapture();
		m_bUsingTool = false;

		InvalidateRect(0, FALSE);

		return TRUE;
	}

	return FALSE;
}

string CDrawWindow::m_sWindowClass;

HBITMAP CDrawWindow::TakeScreenshot()
{
	HBITMAP hResultBitmap;
	
	HDC hDC = GetDC();

	RECT oRect;
	GetClientRect(&oRect);

	HDC hBitmapDC = CreateCompatibleDC(hDC);
	hResultBitmap = CreateCompatibleBitmap(hDC, oRect.right, oRect.bottom);
	HBITMAP	hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, hResultBitmap);

	DoPaint(hBitmapDC, false);

	SelectObject(hBitmapDC, hOldBitmap);

	DeleteDC(hBitmapDC);
	ReleaseDC(hDC);

	return hResultBitmap;
}

