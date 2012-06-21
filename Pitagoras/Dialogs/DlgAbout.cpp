#include "DlgAbout.h"

CDlgAbout::CDlgAbout(CWindow* pParent):
	CDialog(pParent)
{
}

INT_PTR CALLBACK CDlgAbout::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return CDialog::WndProc(hWnd, nMsg, wParam, lParam);
}


