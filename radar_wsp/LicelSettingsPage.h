#pragma once
#include "afxwin.h"


class CLicelSettingsPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(CLicelSettingsPage)

public:
	CLicelSettingsPage();
	virtual ~CLicelSettingsPage();

	enum { IDD = IDD_LICELSETTINGSDLG };

protected:
	CComboBox m_ComboTrigType;
	CComboBox m_ComboInputType;

	int m_TrigType;
	int m_InputRangeType;
	unsigned int m_DiscriminatorLevel;
	unsigned int m_SampleFreq;
	unsigned int m_LaserFreq;
	unsigned int m_CurvePointNum;
	unsigned int m_CurveFreq;
	void LoadConfigs();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void SaveConfigs();
	afx_msg void OnEnUpdateEditDiscriminatorLevel();
	afx_msg void OnEnUpdateEditSampleFreq();
	afx_msg void OnEnUpdateEditLaserFreq();
	afx_msg void OnEnUpdateEditCurvePoint();
	afx_msg void OnEnUpdateEditCurveFreq();
	afx_msg void OnCbnSelchangeComboTrigtype();
	afx_msg void OnCbnSelchangeComboInputrange();
};
