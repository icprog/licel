// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "Resource.h"

#ifdef _DEBUG
#pragma comment(lib,"../Debug/RelModel.lib")
#else 
#pragma comment(lib,"../Release/RelModel.lib")
#endif
#include "../RelModel/Sample.h"
#include "../RelModel/RadarStation.h"
#include "../RelModel/Channel.h"
#include "../RelModel/ChannelProperty.h"
#include "../RelModel/RecordProperty.h"
#include "../RelModel/DatRecord.h"
#include "../RelModel/DatModel.h"

#ifdef _DEBUG
#pragma comment(lib,"../Debug/algcpp.lib")
#else 
#pragma comment(lib,"../Release/algcpp.lib")
#endif
#include "../algcpp/algcpp.h"

#ifdef _DEBUG
#pragma comment(lib,"../Debug/LicelAcq.lib")
#else
#pragma comment(lib,"../Release/LicelAcq.lib")
#endif
#include "../LicelAcq/LicelAcqCtl.h"

#include <afxcontrolbars.h>

