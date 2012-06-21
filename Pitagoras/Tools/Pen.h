#ifndef PEN_H
#define PEN_H

#include "Headers/Headers.h"
#include "Tool.h"

class CPen :
	public CTool
{
public:
	CPen(COLORREF nForegroundColor, COLORREF nBackgroundColor);

	virtual void OnToolStart(UINT nModifier, short nX, short nY);
	virtual void OnToolUse(UINT nModifier, short nX, short nY);
	virtual void OnToolEnd(UINT nModifier, short nX, short nY);
	virtual void OnPaint(HDC hDC);

	virtual LPSTR GetToolCursor() { return MAKEINTRESOURCE(IDC_CURSOR_PEN); }

private:
	list<POINTS> m_oPointList;
};

#endif