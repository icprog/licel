#pragma once
#include "afxwin.h"


// CSaveSettingsDlg 对话框

class CSaveSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveSettingsDlg)

public:
	CSaveSettingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveSettingsDlg();

// 对话框数据
	enum { IDD = IDD_SAVESETTINGSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboSaveStyle;
	CXTPBrowseEdit m_DirBrowserEdit;
	int m_SaveStyle;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboSavestyle();
};
