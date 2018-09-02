#pragma once
#include "afxwin.h"


// CSaveSettingsDlg �Ի���

class CSaveSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveSettingsDlg)

public:
	CSaveSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSaveSettingsDlg();

// �Ի�������
	enum { IDD = IDD_SAVESETTINGSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
