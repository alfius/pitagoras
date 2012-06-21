#ifndef FILL_H
#define FILL_H

#include "Headers/Headers.h"
#include "Tool.h"

class CFill :
	public CTool
{
public:
	CFill(COLORREF nForegroundColor, COLORREF nBackgroundColor);

	virtual bool PreviewTool() { return false; }
	virtual void OnToolStart(UINT nModifier, short nX, short nY);
	virtual void OnToolUse(UINT nModifier, short nX, short nY);
	virtual void OnPaint(HDC hDC);

	virtual LPSTR GetToolCursor() { return MAKEINTRESOURCE(IDC_CURSOR_FILL); }

private:
	POINTS m_oOrigin;
	vector<POINTS> m_oPaintQueue;
};

#endif