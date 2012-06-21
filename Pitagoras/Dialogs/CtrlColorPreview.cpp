#include "CtrlColorPreview.h"
#include "App/App.h"

BOOL CCtrlColorPreview::RegisterClass()
{
	if (m_sWindowClass.length() == 0)
		CApp::GetInstance()->LoadString(IDS_CTRL_COLOR_PREVIEW, &m_sWindowClass);

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

BOOL CCtrlColorPreview::Create(CDialog* pParent)
{
	CreateWindow
	(
		m_sWindowClass.c_str(),
		"", 
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

LRESULT CALLBACK CCtrlColorPreview::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
		case WM_PAINT:
			return OnPaint();
	}

	return CWindow::WndProc(hWnd, nMsg, wParam, lParam);
}

LRESULT CCtrlColorPreview::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(m_hWnd, &ps);

	RECT oRect;
	GetClientRect(&oRect);

	HBRUSH hBrush = CreateSolidBrush(m_nColor);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Rectangle(hDC, oRect.left - 1, oRect.top - 1, oRect.right + 1, oRect.bottom + 1);

	SelectObject(hDC, hOldBrush);
	DeleteObject(hOldBrush);

	EndPaint(m_hWnd, &ps);

	return TRUE;
}

string CCtrlColorPreview::m_sWindowClass;

