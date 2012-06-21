#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "Headers/Headers.h"
#include "Window.h"

class CToolBar :
	public CWindow
{
public:
	BOOL Create(CWindow* pParent);
	BOOL Destroy();
	BOOL Update();
	void EnableButton(int nID, bool bEnable = true);

protected:
	HBITMAP m_hBitmapToolBar;
	HICON m_hColorIcon;
	HIMAGELIST m_hImageList;

private:
	string m_sTitle;

	enum
	{
		CIPT_TRANSPARENT,
		CIPT_BACKGROUND,
		CIPT_FOREGROUND,
		CIPT_SHADOW1,
		CIPT_SHADOW2,
		CIPT_BORDER
	};

	void CreateColorIcon();
	int ColorIconPixelType(int nX, int nY);
};

#endif