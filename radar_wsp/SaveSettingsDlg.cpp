// SaveSettingsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "SaveSettingsDlg.h"
#include "afxdialogex.h"


// CSaveSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CSaveSettingsDlg, CDialogEx)

CSaveSettingsDlg::CSaveSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSaveSettingsDlg::IDD, pParent)
{

}

CSaveSettingsDlg::~CSaveSettingsDlg()
{
}

void CSaveSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SAVESTYLE, m_ComboSaveStyle);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_DirBrowserEdit);
}

BEGIN_MESSAGE_MAP(CSaveSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSaveSettingsDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_SAVESTYLE, &CSaveSettingsDlg::OnCbnSelchangeComboSavestyle)
END_MESSAGE_MAP()


// CSaveSettingsDlg 消息处理程序
BOOL CSaveSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ComboSaveStyle.AddString(_T("本地文件"));

	CString iniFile = Cradar_wspApp::GetAppPath() + _T("configs\\settings.ini");
	int cur = GetPrivateProfileInt(_T("save"),_T("dev"),0,iniFile);
	m_ComboSaveStyle.SetCurSel((cur >=0 && cur<2)?cur:0);
	
	m_SaveStyle = m_ComboSaveStyle.GetCurSel();
	if(m_SaveStyle == 1)
		GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(FALSE);

	unsigned char cBuf[_MAX_PATH];
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("save"),_T("savepath"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	CString keyString;
	keyString.Append((LPWSTR)cBuf);
	m_DirBrowserEdit.Initialize(this, BES_XTP_CHOOSEDIR);
	m_DirBrowserEdit.SetWindowText(keyString);

	return TRUE; 
}

void CSaveSettingsDlg::OnBnClickedOk()
{
	CString iniFile = Cradar_wspApp::GetAppPath() + _T("configs\\settings.ini");
	CString valStr;
	valStr.Format(_T("%d"),m_SaveStyle);
	WritePrivateProfileString(_T("save"),_T("dev"),valStr,iniFile);

	if(m_SaveStyle == 0)
	{
		m_DirBrowserEdit.GetWindowText(valStr);
		WritePrivateProfileString(_T("save"),_T("savepath"),valStr,iniFile);
	}

	CDialogEx::OnOK();
}

void CSaveSettingsDlg::OnCbnSelchangeComboSavestyle()
{
	m_SaveStyle = m_ComboSaveStyle.GetCurSel();
	if(m_SaveStyle == 1)
		GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(FALSE);
	else GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(TRUE);
}
