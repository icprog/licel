#pragma once
#include "afxwin.h"


// CLicelSettingsPage �Ի���

class CLicelSettingsPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(CLicelSettingsPage)

public:
	CLicelSettingsPage();
	virtual ~CLicelSettingsPage();

// �Ի�������
	enum { IDD = IDD_LICELSETTINGSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboTrigType;
	CComboBox m_ComboInputType;
	virtual BOOL OnInitDialog();
};
