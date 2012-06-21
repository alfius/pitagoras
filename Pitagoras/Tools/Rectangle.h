#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Headers/Headers.h"
#include "Tool.h"

class CRectangle :
	public CTool
{
public:
	CRectangle(COLORREF nForegroundColor, COLORREF nBackgroundColor);

	virtual void OnToolStart(UINT nModifier, short nX, short nY);
	virtual void OnToolUse(UINT nModifier, short nX, short nY);
	virtual void OnToolEnd(UINT nModifier, short nX, short nY);
	virtual void OnPaint(HDC hDC);

protected:
	POINTS m_oOrigin;
	RECT m_oRect;
};

#endif