#pragma once


// CAxisSettingsDlg �Ի���

class CAxisSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAxisSettingsDlg)

public:
	CAxisSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAxisSettingsDlg();

// �Ի�������
	enum { IDD = IDD_AXISSETTINGSDLG };
	//enum DLG_AXIS_TYPE{ DLG_AXIS_H_SETTINGS,DLG_AXIS_V_SETTINGS};

public:
	//DLG_AXIS_TYPE m_AxisType;
	BOOL m_bLogCheckEnable;
	CString m_Title;
	//double m_LimitMax;
	//double m_LimitMin;
	double m_Max;
	double m_Min;
	BOOL m_bLogEnable;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
