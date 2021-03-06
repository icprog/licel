//

#include "stdafx.h"
#include "radar_wsp.h"
#include "CaptureTaskDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CCaptureTaskDlg, CDialogEx)

CCaptureTaskDlg::CCaptureTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCaptureTaskDlg::IDD, pParent)
{
	CString iniFile = ((Cradar_wspApp *)AfxGetApp())->GetAppPath();
	CString path = iniFile + _T("configs\\settings.ini");
	m_Period = GetPrivateProfileInt(_T("capture"),_T("period"),60,path);
}

void CCaptureTaskDlg::SaveConfigs()
{
	CString iniFile = ((Cradar_wspApp *)AfxGetApp())->GetAppPath();
	iniFile += _T("configs\\settings.ini");
	CString strTemp;
	strTemp.Format(_T("%d"),m_Period);
	WritePrivateProfileString(_T("capture"),_T("period"),strTemp,iniFile);
}

CCaptureTaskDlg::~CCaptureTaskDlg()
{
	
}

void CCaptureTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_ComboPeriod);
}

BEGIN_MESSAGE_MAP(CCaptureTaskDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PERIOD, &CCaptureTaskDlg::OnCbnSelchangeComboPeriod)
	ON_BN_CLICKED(IDOK, &CCaptureTaskDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CCaptureTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for(int i=0;i<CAcquireHelper::AcquirePeriod_Sum;i++)
	{
		CString caption(CAcquireHelper::AcquirePeriod_Caption[i]);
		int index = m_ComboPeriod.AddString(caption);
		if(m_Period == CAcquireHelper::AcquirePeriod_Value[i])
			m_ComboPeriod.SetCurSel(index);
	}

	return TRUE;
}

void CCaptureTaskDlg::OnCbnSelchangeComboPeriod()
{
	int index = m_ComboPeriod.GetCurSel();
	m_Period = CAcquireHelper::AcquirePeriod_Value[index];
}


void CCaptureTaskDlg::OnBnClickedOk()
{
	SaveConfigs();
	CDialogEx::OnOK();
}
