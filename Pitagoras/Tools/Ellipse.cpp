#include "Ellipse.h"

CEllipse::CEllipse(COLORREF nForegroundColor, COLORREF nBackgroundColor):
	CRectangle(nForegroundColor, nBackgroundColor)
{
}

void CEllipse::OnPaint(HDC hDC)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, m_nForegroundColor);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	HBRUSH hBrush = CreateSolidBrush(m_nBackgroundColor);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Ellipse(hDC, m_oRect.left, m_oRect.top, m_oRect.right, m_oRect.bottom);

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

