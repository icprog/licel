#pragma once
#include "afxcmn.h"


// CRecordDlg �Ի���

class CRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordDlg();

// �Ի�������
	enum { IDD = IDD_RECORDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
