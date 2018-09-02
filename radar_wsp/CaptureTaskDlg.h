#pragma once
#include "afxwin.h"


class CCaptureTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCaptureTaskDlg)

public:
	CCaptureTaskDlg(CWnd* pParent = NULL);   
	virtual ~CCaptureTaskDlg();

	enum { IDD = IDD_CAPTURESETTINGDLG };

protected:
	unsigned int m_Period;
	CComboBox m_ComboPeriod;
	void SaveConfigs();

public:
	void SetCapturePeriod(unsigned int Period) { m_Period = Period; }
	unsigned int GetCapturePeriod() { return m_Period; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboPeriod();
	afx_msg void OnBnClickedOk();
};
