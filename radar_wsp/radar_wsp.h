// radar_wsp.h : main header file for the radar_wsp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// Cradar_wspApp:
// See radar_wsp.cpp for the implementation of this class
//

class Cradar_wspApp : public CWinApp
{
public:
	Cradar_wspApp();

private:
	ULONG_PTR m_gdiplusToken;

// Overrides
public:
	static CString GetAppPath();
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cradar_wspApp theApp;