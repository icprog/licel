#pragma once
#include "afxcmn.h"


// CRecordDlg 对话框

class CRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordDlg();

// 对话框数据
	enum { IDD = IDD_RECORDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CListCtrl m_List;
	void ResetList();

public:
	void DisplayRecord(CProductHelper::ProductType productType,CDatRecord* pRecord);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
};
