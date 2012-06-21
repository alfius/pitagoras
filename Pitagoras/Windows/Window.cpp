#include "Window.h"

LRESULT CALLBACK CWindow::BaseWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
		case WM_NCCREATE:
			SetWindowLong(hWnd, GWL_USERDATA, (long)((LPCREATESTRUCT)lParam)->lpCreateParams);
			((CWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams)->m_hWnd = hWnd;

		default:
			CWindow* pWnd = (CWindow*)GetWindowLong(hWnd, GWL_USERDATA);
			if (pWnd)
			{
				return pWnd->WndProc(hWnd, nMsg, wParam, lParam);
			}
			else return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return FALSE;
}

LRESULT CALLBACK CWindow::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}

LRESULT CWindow::SendMessage(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return ::SendMessage(m_hWnd, nMsg, wParam, lParam);
}

BOOL CWindow::MoveWindow(int nX, int nY, int nWidth, int nHeight, BOOL bRepaint)
{
	return ::MoveWindow(m_hWnd, nX, nY, nWidth, nHeight, bRepaint);
}

BOOL CWindow::ShowWindow(int nCmdShow)
{
	return ::ShowWindow(m_hWnd, nCmdShow);
}

BOOL CWindow::UpdateWindow()
{
	return ::UpdateWindow(m_hWnd);
}

BOOL CWindow::GetClientRect(LPRECT pRect)
{
	return ::GetClientRect(m_hWnd, pRect);
}

void CWindow::InvalidateRect(LPRECT pRect, BOOL bErase)
{
	::InvalidateRect(m_hWnd, pRect, bErase);
}

HDC CWindow::GetDC()
{
	return ::GetDC(m_hWnd);
}

int CWindow::ReleaseDC(HDC hDC)
{
	return ::ReleaseDC(m_hWnd, hDC);
}
