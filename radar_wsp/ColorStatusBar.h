#pragma once


// CColorStatusBar

class CColorStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CColorStatusBar)

public:
	CColorStatusBar();
	virtual ~CColorStatusBar();

protected:
	CString m_WarningPaneText;
	BOOL m_bWarning;

public:
	void SetWarningPaneText(CString paneText,BOOL bWarn);

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


