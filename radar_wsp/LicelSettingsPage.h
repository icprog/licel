#pragma once
#include "afxwin.h"


// CLicelSettingsPage 对话框

class CLicelSettingsPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(CLicelSettingsPage)

public:
	CLicelSettingsPage();
	virtual ~CLicelSettingsPage();

// 对话框数据
	enum { IDD = IDD_LICELSETTINGSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboTrigType;
	CComboBox m_ComboInputType;
	virtual BOOL OnInitDialog();
};
