#include "Fill.h"
#include "Utils/BinaryBitmap.h"

CFill::CFill(COLORREF nForegroundColor, COLORREF nBackgroundColor):
	CTool(nForegroundColor, nBackgroundColor)
{
}

void CFill::OnToolStart(UINT nModifier, short nX, short nY)
{
	LONG nPoint = MAKELONG(nX, nY);
	m_oOrigin = MAKEPOINTS(nPoint);
}

void CFill::OnToolUse(UINT nModifier, short nX, short nY)
{
	LONG nPoint = MAKELONG(nX, nY);
	m_oOrigin = MAKEPOINTS(nPoint);
}

void CFill::OnPaint(HDC hDC)
{
	COLORREF nTargetColor = ::GetPixel(hDC, m_oOrigin.x, m_oOrigin.y);
	COLORREF nNewColor = m_nForegroundColor;

	if (nTargetColor == nNewColor)
		return;

	if (m_oOrigin.x < 0 || m_oOrigin.x >= GetDeviceCaps(hDC, HORZRES))
		return;

	if (m_oOrigin.y < 0 || m_oOrigin.y >= GetDeviceCaps(hDC, VERTRES))
		return;

	HBITMAP hBitmap = 0;
	{
		HBITMAP hTempBitmap = CreateCompatibleBitmap(hDC, GetDeviceCaps(hDC, HORZRES), GetDeviceCaps(hDC, VERTRES));
		hBitmap = (HBITMAP)SelectObject(hDC, hTempBitmap);
		SelectObject(hDC, hBitmap);
		DeleteObject(hTempBitmap);
	}

	CBinaryBitmap oBinaryBitmap(hBitmap);

	m_oPaintQueue.clear();
	m_oPaintQueue.push_back(m_oOrigin);

	for (unsigned int i = 0; i < m_oPaintQueue.size(); i++)
	{
		POINTS oPoint = m_oPaintQueue[i];
		
		if (oBinaryBitmap.GetPixel(oPoint.x, oPoint.y) != nTargetColor)
			continue;

		int nLeftEdge = oPoint.x;
		while (nLeftEdge >= 0 && oBinaryBitmap.GetPixel(nLeftEdge, oPoint.y) == nTargetColor)
			nLeftEdge--;

		int nRightEdge = oPoint.x;
		while (nRightEdge < GetDeviceCaps(hDC, HORZRES) && oBinaryBitmap.GetPixel(nRightEdge, oPoint.y) == nTargetColor)
			nRightEdge++;

		for (int i = nLeftEdge + 1; i < nRightEdge; i++)
		{
			oBinaryBitmap.SetPixel(i, oPoint.y, nNewColor);
		}

		if (oPoint.y - 1 >= 0)
		{
			for (int nXCoord = nLeftEdge + 1; nXCoord != nRightEdge - 1; nXCoord++)
			{
				LONG nTopPoint = MAKELONG(nXCoord, oPoint.y - 1);
				POINTS oTopPoint = MAKEPOINTS(nTopPoint);

				m_oPaintQueue.push_back(oTopPoint);
			}
		}

		if (oPoint.y + 1 < GetDeviceCaps(hDC, VERTRES))
		{
			for (int nXCoord = nLeftEdge + 1; nXCoord != nRightEdge - 1; nXCoord++)
			{
				LONG nBottomPoint = MAKELONG(nXCoord, oPoint.y + 1);
				POINTS oBottomPoint = MAKEPOINTS(nBottomPoint);

				m_oPaintQueue.push_back(oBottomPoint);
			}
		}
	}

	m_oPaintQueue.clear();
}
