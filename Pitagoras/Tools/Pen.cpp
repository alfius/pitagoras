#include "Pen.h"

CPen::CPen(COLORREF nForegroundColor, COLORREF nBackgroundColor):
	CTool(nForegroundColor, nBackgroundColor)
{
}

void CPen::OnToolStart(UINT nModifier, short nX, short nY)
{
	m_oPointList.clear();
	LONG nPoint = MAKELONG(nX, nY);
	m_oPointList.push_back(MAKEPOINTS(nPoint));
}

void CPen::OnToolUse(UINT nModifier, short nX, short nY)
{
	LONG nPoint = MAKELONG(nX, nY);
	m_oPointList.push_back(MAKEPOINTS(nPoint));
}

void CPen::OnToolEnd(UINT nModifier, short nX, short nY)
{
	m_oPointList.clear();
}

void CPen::OnPaint(HDC hDC)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, m_nForegroundColor);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	POINTS oPrevPoint;
	bool bFirst = true;
	for each (POINTS oPoint in m_oPointList)
	{
		if (!bFirst)
		{
			MoveToEx(hDC, oPrevPoint.x, oPrevPoint.y, 0);
			LineTo(hDC, oPoint.x, oPoint.y);
		}
		bFirst = false;
		oPrevPoint = oPoint;
	}

	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

