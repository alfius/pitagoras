#ifndef DLG_COLORS_H
#define DLG_COLORS_H

#include "Headers/Headers.h"
#include "Resource/Resource.h"
#include "Dialog.h"
#include "CtrlColorPreview.h"

class CDlgColors :
	public CDialog
{
public:
	CDlgColors(CWindow* pParent);
	virtual UINT GetTemplateID() { return IDD_COLORS; }

	COLORREF m_nForegroundColor;
	COLORREF m_nBackgroundColor;

protected:
	CCtrlColorPreview* m_pForegroundPreview;
	CCtrlColorPreview* m_pBackgroundPreview;

	virtual INT_PTR CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	virtual INT_PTR OnInitDialog();
	virtual INT_PTR OnSize(UINT nAction, USHORT nNewWidth, USHORT nNewHeight);
	virtual INT_PTR OnSetForeground();
	virtual INT_PTR OnSetBackground();

private:
	bool ShowColorPicker(COLORREF *pColor);
};

#endif