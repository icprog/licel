// RelModel.h : RelModel DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRelModelApp
// �йش���ʵ�ֵ���Ϣ������� RelModel.cpp
//

class CRelModelApp : public CWinApp
{
public:
	CRelModelApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
