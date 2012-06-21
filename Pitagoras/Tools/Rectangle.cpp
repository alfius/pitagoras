#include "Rectangle.h"

CRectangle::CRectangle(COLORREF nForegroundColor, COLORREF nBackgroundColor):
	CTool(nForegroundColor, nBackgroundColor)
{
}

void CRectangle::OnToolStart(UINT nModifier, short nX, short nY)
{
	LONG nPoint = MAKELONG(nX, nY);
	m_oOrigin = MAKEPOINTS(nPoint);

	m_oRect.left = m_oRect.right = nX;
	m_oRect.top = m_oRect.bottom = nY;
}

void CRectangle::OnToolUse(UINT nModifier, short nX, short nY)
{
	if (nModifier & MK_SHIFT)
	{
		m_oRect.left = m_oOrigin.x - (nX - m_oOrigin.x);
		m_oRect.top = m_oOrigin.y - (nY - m_oOrigin.y);
		m_oRect.right = nX;
		m_oRect.bottom = nY;
	}
	else
	{
		m_oRect.left = m_oOrigin.x;
		m_oRect.top = m_oOrigin.y;
		m_oRect.right = nX;
		m_oRect.bottom = nY;
	}
}

void CRectangle::OnToolEnd(UINT nModifier, short nX, short nY)
{
}

void CRectangle::OnPaint(HDC hDC)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, m_nForegroundColor);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	HBRUSH hBrush = CreateSolidBrush(m_nBackgroundColor);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Rectangle(hDC, m_oRect.left, m_oRect.top, m_oRect.right, m_oRect.bottom);

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

