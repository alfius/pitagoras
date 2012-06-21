#include "DlgColors.h"
#include "App/App.h"

CDlgColors::CDlgColors(CWindow* pParent):
	CDialog(pParent)
{
}

INT_PTR CALLBACK CDlgColors::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		return OnInitDialog();

	case WM_SIZE:
		return OnSize(wParam, LOWORD(lParam), HIWORD(lParam));

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDC_SET_FOREGROUND:
			return OnSetForeground();

		case IDC_SET_BACKGROUND:
			return OnSetBackground();
		}

		break;
	}
	return CDialog::WndProc(hWnd, nMsg, wParam, lParam);
}

INT_PTR CDlgColors::OnInitDialog()
{
	m_pForegroundPreview = new CCtrlColorPreview();
	if (!m_pForegroundPreview->Create(this))
	{
		return FALSE;
	}

	m_pForegroundPreview->m_nColor = m_nForegroundColor;

	m_pBackgroundPreview = new CCtrlColorPreview();
	if (!m_pBackgroundPreview->Create(this))
	{
		return FALSE;
	}

	m_pBackgroundPreview->m_nColor = m_nBackgroundColor;

	OnSize(0, 0, 0);

	return (INT_PTR)TRUE;
}

INT_PTR CDlgColors::OnSize(UINT nAction, USHORT nNewWidth, USHORT nNewHeight)
{
	HWND hWndButtonForeground = GetDlgItem(m_hWnd, IDC_SET_FOREGROUND);
	HWND hWndButtonBackground = GetDlgItem(m_hWnd, IDC_SET_BACKGROUND);
	if (hWndButtonForeground && hWndButtonBackground)
	{
		const int nMargin = 5;
		const int nDiff = 2;

		WINDOWINFO oWI;
		GetWindowInfo(m_hWnd, &oWI);

		RECT oRect;
		GetWindowRect(hWndButtonForeground, &oRect);

		oRect.bottom -= oWI.rcClient.top;
		oRect.left -= oWI.rcClient.left;
		oRect.right -= oWI.rcClient.left;
		oRect.top -= oWI.rcClient.top;

		const int nWidth = oRect.bottom - oRect.top - (2 * nDiff);

		m_pForegroundPreview->MoveWindow(oRect.left - (nWidth + nMargin) + nDiff, oRect.top + nDiff, nWidth, nWidth);

		GetWindowRect(hWndButtonBackground, &oRect);

		oRect.bottom -= oWI.rcClient.top;
		oRect.left -= oWI.rcClient.left;
		oRect.right -= oWI.rcClient.left;
		oRect.top -= oWI.rcClient.top;

		m_pBackgroundPreview->MoveWindow(oRect.left - (nWidth + nMargin) + nDiff, oRect.top + nDiff, nWidth, nWidth);
	}

	return (INT_PTR)TRUE;
}

INT_PTR CDlgColors::OnSetForeground()
{
	if (ShowColorPicker(&m_nForegroundColor))
	{
		m_pForegroundPreview->m_nColor = m_nForegroundColor;
		m_pForegroundPreview->InvalidateRect();
	}

	return (INT_PTR)true;
}

INT_PTR CDlgColors::OnSetBackground()
{
	if (ShowColorPicker(&m_nBackgroundColor))
	{
		m_pBackgroundPreview->m_nColor = m_nBackgroundColor;
		m_pBackgroundPreview->InvalidateRect();
	}

	return (INT_PTR)true;
}

bool CDlgColors::ShowColorPicker(COLORREF *pColor)
{
	CHOOSECOLOR oColorPicker;
	memset(&oColorPicker, 0, sizeof(CHOOSECOLOR));
	oColorPicker.lStructSize = sizeof(CHOOSECOLOR);
	oColorPicker.hwndOwner = m_hWnd;
	oColorPicker.rgbResult = *pColor;
	oColorPicker.lpCustColors = CApp::GetInstance()->m_oCustomColors;
	oColorPicker.Flags = CC_RGBINIT | CC_FULLOPEN;
	if (ChooseColor(&oColorPicker))
	{
		*pColor = oColorPicker.rgbResult;
		return true;
	}
	return false;
}

