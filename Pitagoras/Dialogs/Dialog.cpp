#include "Dialog.h"
#include "App/App.h"

CDialog::CDialog(CWindow* pParent):
	m_pParent(pParent)
{
}

INT_PTR CALLBACK CDialog::BaseWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
		case WM_INITDIALOG:
			((CDialog*)lParam)->m_hWnd = hWnd;
			SetWindowLong(hWnd, GWL_USERDATA, (long)lParam);

		default:
			CDialog* pDlg = (CDialog*)GetWindowLong(hWnd, GWL_USERDATA);
			if (pDlg)
			{
				return pDlg->WndProc(hWnd, nMsg, wParam, lParam);
			}
	}
	return FALSE;
}

INT_PTR CALLBACK CDialog::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return (INT_PTR)FALSE;
}

INT_PTR CDialog::DoModal()
{
	return DialogBoxParam(CApp::GetInstance()->m_hInstance, MAKEINTRESOURCE(GetTemplateID()), m_pParent->m_hWnd, BaseWndProc, (LPARAM)this);
}

