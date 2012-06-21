#ifndef DIALOG_H
#define DIALOG_H

#include "Headers/Headers.h"
#include "Windows/Window.h"

class CDialog
{
public:
	HWND m_hWnd;

	CDialog(CWindow* pParent);

	static INT_PTR CALLBACK BaseWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	virtual INT_PTR CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	virtual UINT GetTemplateID() = 0;
	virtual INT_PTR DoModal();

private:
	CWindow* m_pParent;
};

#endif