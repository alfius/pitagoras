#include "ToolBar.h"
#include "App/App.h"

BOOL CToolBar::Create(CWindow* pParent)
{
	CApp::GetInstance()->LoadString(IDS_TOOLBAR_TITLE, &m_sTitle);

	m_hWnd = CreateWindowEx
	(
		WS_EX_TOOLWINDOW,
		"ToolbarWindow32",
		m_sTitle.c_str(), 
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT,
		0, 0, 1, 1, 
		pParent->m_hWnd, 
		(HMENU)-1, 
		CApp::GetInstance()->m_hInstance, 
		this
	);

	if (!m_hWnd)
	{
		return FALSE;
	}

	m_hImageList = ImageList_Create(32,32,ILC_COLOR32,0,32);
	if (!m_hImageList)
	{
		return FALSE;
	}

	m_hBitmapToolBar = CApp::GetInstance()->LoadBitmap(IDB_TOOLBAR, 224, 32);
	if (!m_hBitmapToolBar)
	{
		return FALSE;
	}

	CreateColorIcon();

	ImageList_Add(m_hImageList, m_hBitmapToolBar, 0);
	ImageList_AddIcon(m_hImageList, m_hColorIcon);

	SendMessage(TB_SETIMAGELIST, 0, (LPARAM)m_hImageList);

	
	const int nButtons = 10;
	TBBUTTON buttons[nButtons];
	memset(&buttons, 0, nButtons * sizeof(TBBUTTON));
	
	buttons[0].iBitmap = 0;
	buttons[0].idCommand = IDM_NEW;
	buttons[0].fsStyle = TBSTYLE_BUTTON;
	buttons[0].fsState = TBSTATE_ENABLED;
	buttons[0].iString = (int)"Nuevo";

	buttons[1].iBitmap = 1;
	buttons[1].idCommand = IDM_OPEN;
	buttons[1].fsStyle = TBSTYLE_BUTTON;
	buttons[1].fsState = TBSTATE_ENABLED;
	buttons[1].iString = (int)"Abrir";

	buttons[2].iBitmap = 2;
	buttons[2].idCommand = IDM_SAVE;
	buttons[2].fsStyle = TBSTYLE_BUTTON;
	buttons[2].fsState = TBSTATE_ENABLED;
	buttons[2].iString = (int)"Guardar";

	buttons[3].fsStyle = TBSTYLE_SEP;

	buttons[4].iBitmap = 3;
	buttons[4].idCommand = IDM_PEN;
	buttons[4].fsStyle = TBSTYLE_BUTTON;
	buttons[4].fsState = TBSTATE_ENABLED;
	buttons[4].iString = (int)"Lápiz";

	buttons[5].iBitmap = 4;
	buttons[5].idCommand = IDM_RECT;
	buttons[5].fsStyle = TBSTYLE_BUTTON;
	buttons[5].fsState = TBSTATE_ENABLED;
	buttons[5].iString = (int)"Rectángulo";

	buttons[6].iBitmap = 5;
	buttons[6].idCommand = IDM_ELLIPSE;
	buttons[6].fsStyle = TBSTYLE_BUTTON;
	buttons[6].fsState = TBSTATE_ENABLED;
	buttons[6].iString = (int)"Elipse";

	buttons[7].iBitmap = 6;
	buttons[7].idCommand = IDM_FILL;
	buttons[7].fsStyle = TBSTYLE_BUTTON;
	buttons[7].fsState = TBSTATE_ENABLED;
	buttons[7].iString = (int)"Rellenar";

	buttons[8].fsStyle = TBSTYLE_SEP;

	buttons[9].iBitmap = 7;
	buttons[9].idCommand = IDM_COLORS;
	buttons[9].fsStyle = TBSTYLE_BUTTON;
	buttons[9].fsState = TBSTATE_ENABLED;
	buttons[9].iString = (int)"Colores";

	SendMessage(TB_ADDBUTTONS, sizeof(buttons)/sizeof(TBBUTTON), (LPARAM)&buttons);
	SendMessage(TB_AUTOSIZE, 0, 0);

	return TRUE;
}

BOOL CToolBar::Destroy()
{
	BOOL bResult = TRUE;

	bResult &= DeleteObject(m_hBitmapToolBar);
	bResult &= DestroyIcon(m_hColorIcon);
	bResult &= ImageList_Destroy(m_hImageList);

	return bResult;
}

BOOL CToolBar::Update()
{
	BOOL bResult = TRUE;

	bResult &= ImageList_Remove(m_hImageList, 7);

	if (m_hColorIcon)
		bResult &= DestroyIcon(m_hColorIcon);
	CreateColorIcon();

	bResult &= ImageList_AddIcon(m_hImageList, m_hColorIcon);

	InvalidateRect();

	return bResult;
}

void CToolBar::EnableButton(int nID, bool bEnable)
{
	TBBUTTONINFO oButtonInfo;
	oButtonInfo.cbSize = sizeof(TBBUTTONINFO);
	oButtonInfo.dwMask = TBIF_STATE;
	oButtonInfo.fsState = bEnable ? TBSTATE_ENABLED : 0;

	SendMessage(TB_SETBUTTONINFO, nID, (LPARAM)&oButtonInfo);
}

void CToolBar::CreateColorIcon()
{
	BYTE oAndBits[32*32*4];
	memset(oAndBits, 0, 32*32*4);
	BYTE oXorBits[32*32*4];

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			switch (ColorIconPixelType(i, j))
			{
			case CIPT_FOREGROUND:
				oXorBits[(i * 32 + j) * 4] = GetBValue(CApp::GetInstance()->m_nForegroundColor);
				oXorBits[(i * 32 + j) * 4 + 1] = GetGValue(CApp::GetInstance()->m_nForegroundColor);
				oXorBits[(i * 32 + j) * 4 + 2] = GetRValue(CApp::GetInstance()->m_nForegroundColor);
				oXorBits[(i * 32 + j) * 4 + 3] = 255;
				break;

			case CIPT_BACKGROUND:
				oXorBits[(i * 32 + j) * 4] = GetBValue(CApp::GetInstance()->m_nBackgroundColor);
				oXorBits[(i * 32 + j) * 4 + 1] = GetGValue(CApp::GetInstance()->m_nBackgroundColor);
				oXorBits[(i * 32 + j) * 4 + 2] = GetRValue(CApp::GetInstance()->m_nBackgroundColor);
				oXorBits[(i * 32 + j) * 4 + 3] = 255;
				break;

			case CIPT_BORDER:
				oXorBits[(i * 32 + j) * 4] = 160;
				oXorBits[(i * 32 + j) * 4 + 1] = 160;
				oXorBits[(i * 32 + j) * 4 + 2] = 160;
				oXorBits[(i * 32 + j) * 4 + 3] = 255;
				break;

			case CIPT_SHADOW1:
				oXorBits[(i * 32 + j) * 4] = 0;
				oXorBits[(i * 32 + j) * 4 + 1] = 0;
				oXorBits[(i * 32 + j) * 4 + 2] = 0;
				oXorBits[(i * 32 + j) * 4 + 3] = 64;
				break;

			case CIPT_SHADOW2:
				oXorBits[(i * 32 + j) * 4] = 0;
				oXorBits[(i * 32 + j) * 4 + 1] = 0;
				oXorBits[(i * 32 + j) * 4 + 2] = 0;
				oXorBits[(i * 32 + j) * 4 + 3] = 32;
				break;

			case CIPT_TRANSPARENT:
				oXorBits[(i * 32 + j) * 4] = 0;
				oXorBits[(i * 32 + j) * 4 + 1] = 0;
				oXorBits[(i * 32 + j) * 4 + 2] = 0;
				oXorBits[(i * 32 + j) * 4 + 3] = 0;
				break;
			}
		}
	}
	m_hColorIcon = CreateIcon(CApp::GetInstance()->m_hInstance, 32, 32, 1, 32, oAndBits, oXorBits);
}

int CToolBar::ColorIconPixelType(int nX, int nY)
{
	if (nX >= 5 && nX <= 20 && nY >= 5 && nY <= 20)
		return CIPT_FOREGROUND;

	if (nX >= 4 && nX <= 21 && nY >= 4 && nY <= 21)
		return CIPT_BORDER;

	if (nX >= 13 && nX <= 27 && nY >= 13 && nY <= 27)
		return CIPT_BACKGROUND;

	if (nX >= 12 && nX <= 28 && nY >= 12 && nY <= 28)
		return CIPT_BORDER;

	if (nX >= 5 && nX <= 22 && nY >= 5 && nY <= 22)
		return CIPT_SHADOW1;

	if (nX >= 13 && nX <= 29 && nY >= 13 && nY <= 29)
		return CIPT_SHADOW1;

	if (nX >= 6 && nX <= 23 && nY >= 6 && nY <= 23)
		return CIPT_SHADOW2;

	if (nX >= 14 && nX <= 30 && nY >= 14 && nY <= 30)
		return CIPT_SHADOW2;

	return CIPT_TRANSPARENT;
}

