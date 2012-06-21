#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "Headers/Headers.h"
#include "Dialog.h"
#include "Resource/resource.h"

class CDlgAbout :
	public CDialog
{
public:
	CDlgAbout(CWindow* pParent);

	virtual UINT GetTemplateID() { return IDD_ABOUTBOX; }
	virtual INT_PTR CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif