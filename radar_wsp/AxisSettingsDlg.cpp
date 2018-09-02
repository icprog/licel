// AxisSettingsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "AxisSettingsDlg.h"
#include "afxdialogex.h"


// CAxisSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CAxisSettingsDlg, CDialogEx)

CAxisSettingsDlg::CAxisSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAxisSettingsDlg::IDD, pParent)
{
	//m_LimitMax = 30;
	//m_LimitMin = 0;
	m_Max = 30;
	m_Min = 0;
}

CAxisSettingsDlg::~CAxisSettingsDlg()
{
}

void CAxisSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CAxisSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAxisSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAxisSettingsDlg 消息处理程序


BOOL CAxisSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(m_Title);

	CString strTemp;
	strTemp.Format(_T("%.6f"),m_Max);
	GetDlgItem(IDC_MAXEDIT)->SetWindowText(strTemp);
	strTemp.Format(_T("%.6f"),m_Min);
	GetDlgItem(IDC_MINEDIT)->SetWindowText(strTemp);

	if(!m_bLogCheckEnable)
		GetDlgItem(IDC_LOGCHECK)->EnableWindow(FALSE);
	else 
	{
		GetDlgItem(IDC_LOGCHECK)->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_LOGCHECK))->SetCheck(m_bLogEnable);
	}

	return TRUE;
}


void CAxisSettingsDlg::OnBnClickedOk()
{
	CString strTemp;
	GetDlgItem(IDC_MAXEDIT)->GetWindowText(strTemp);
	m_Max = _ttof((LPTSTR)(LPCTSTR)strTemp);
	GetDlgItem(IDC_MINEDIT)->GetWindowText(strTemp);
	m_Min = _ttof((LPTSTR)(LPCTSTR)strTemp);

	m_bLogEnable = ((CButton*)GetDlgItem(IDC_LOGCHECK))->GetCheck();
	CDialogEx::OnOK();
}
