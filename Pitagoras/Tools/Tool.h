#ifndef TOOL_H
#define TOOL_H

#include "Headers/Headers.h"
#include "Resource/resource.h"

class CTool
{
public:
	COLORREF m_nForegroundColor;
	COLORREF m_nBackgroundColor;

	CTool(COLORREF nForegroundColor, COLORREF nBackgroundColor):
		m_nForegroundColor(nForegroundColor),
		m_nBackgroundColor(nBackgroundColor)
	{
	}

	virtual bool PreviewTool() { return true; }
	virtual void OnToolStart(UINT nModifier, short nX, short nY) {}
	virtual void OnToolUse(UINT nModifier, short nX, short nY) {}
	virtual void OnToolEnd(UINT nModifier, short nX, short nY) {}
	virtual void OnPaint(HDC hDC) {}

	virtual LPSTR GetToolCursor() { return MAKEINTRESOURCE(IDC_CURSOR_RECT); }
};

#endif