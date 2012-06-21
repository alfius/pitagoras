#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Headers/Headers.h"
#include "Rectangle.h"

class CEllipse :
	public CRectangle
{
public:
	CEllipse(COLORREF nForegroundColor, COLORREF nBackgroundColor);
	
	virtual void OnPaint(HDC hDC);
};

#endif