// LicelAcq.h : LicelAcq DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLicelAcqApp
// �йش���ʵ�ֵ���Ϣ������� LicelAcq.cpp
//

class CLicelAcqApp : public CWinApp
{
public:
	CLicelAcqApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
