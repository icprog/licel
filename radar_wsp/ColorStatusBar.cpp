// ColorStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "ColorStatusBar.h"


// CColorStatusBar

IMPLEMENT_DYNAMIC(CColorStatusBar, CStatusBar)

CColorStatusBar::CColorStatusBar()
{
	m_bWarning = FALSE;
	m_WarningPaneText = _T("");
}

CColorStatusBar::~CColorStatusBar()
{
}


BEGIN_MESSAGE_MAP(CColorStatusBar, CStatusBar)
END_MESSAGE_MAP()



// CColorStatusBar 消息处理程序

void CColorStatusBar::SetWarningPaneText(CString paneText,BOOL bWarn)
{
	m_WarningPaneText = paneText;
	m_bWarning = bWarn;
}



void CColorStatusBar::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->itemID != 4)
		return;

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	dc.SetBkMode(TRANSPARENT);
	CRect rect(&lpDrawItemStruct->rcItem);
	if(m_bWarning)
		dc.SetTextColor(RGB(255,0,0));
	else dc.SetTextColor(RGB(0,0,0));
    dc.TextOut(rect.left+20, rect.top,m_WarningPaneText); 
	dc.Detach();
}
