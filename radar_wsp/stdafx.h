// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0500       // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500     // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINDOWS      // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE           // Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400    // Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#ifdef _DEBUG
#pragma comment(lib,"ToolkitPro1531vc100UD.lib")
#else
#pragma comment(lib,"ToolkitPro1531vc100U.lib")
#endif
#include <XTToolkitPro.h>    // Xtreme Toolkit Pro components

#include <CSLWaterfall.h>
#include <CSLScope.h>

#ifdef _DEBUG
#pragma comment(lib,"../Debug/RelModel.lib")
#else
#pragma comment(lib,"../Release/RelModel.lib")
#endif
#include "../RelModel/DatModel.h"
#include "../RelModel/DatRecord.h"
#include "../RelModel/Sample.h"

#include <afxcontrolbars.h>

#include "../RadarKernal/ProductHelper.h"
#include "../RadarKernal/AcquireHelper.h"
//#include "../RadarKernal/ServoCtlHelper.h"
#ifdef _DEBUG
#pragma comment(lib,"../Debug/RadarKernal.lib")
#else 
#pragma comment(lib,"../Release/RadarKernal.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib,"../Debug/LicelAcq.lib")
#else
#pragma comment(lib,"../Release/LicelAcq.lib")
#endif
#include "../LicelAcq/LicelAcqCtl.h"

#define WM_DATABROWSER_LISTUPDATE WM_USER+0x01
#define WM_CAPTURETASK_EVENT WM_USER+0x03

#define MY_ACTIVITY_LED_TIMER_ID (WM_USER + 0x100)