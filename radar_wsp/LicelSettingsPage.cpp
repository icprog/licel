// LicelSettingsPage.cpp : 实现文件
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "LicelSettingsPage.h"
#include "afxdialogex.h"


// CLicelSettingsPage 对话框

IMPLEMENT_DYNAMIC(CLicelSettingsPage, CXTPPropertyPage)

CLicelSettingsPage::CLicelSettingsPage()
	: CXTPPropertyPage(CLicelSettingsPage::IDD)
{
	LoadConfigs();
}

CLicelSettingsPage::~CLicelSettingsPage()
{
}

void CLicelSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TRIGTYPE, m_ComboTrigType);
	DDX_Control(pDX, IDC_COMBO_INPUTRANGE, m_ComboInputType);
}


BEGIN_MESSAGE_MAP(CLicelSettingsPage, CXTPPropertyPage)
	ON_EN_UPDATE(IDC_EDIT_DISCRIMINATOR_LEVEL, &CLicelSettingsPage::OnEnUpdateEditDiscriminatorLevel)
	ON_EN_UPDATE(IDC_EDIT_SAMPLE_FREQ, &CLicelSettingsPage::OnEnUpdateEditSampleFreq)
	ON_EN_UPDATE(IDC_EDIT_LASER_FREQ, &CLicelSettingsPage::OnEnUpdateEditLaserFreq)
	ON_EN_UPDATE(IDC_EDIT_CURVE_POINT, &CLicelSettingsPage::OnEnUpdateEditCurvePoint)
	ON_EN_UPDATE(IDC_EDIT_CURVE_FREQ, &CLicelSettingsPage::OnEnUpdateEditCurveFreq)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGTYPE, &CLicelSettingsPage::OnCbnSelchangeComboTrigtype)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUTRANGE, &CLicelSettingsPage::OnCbnSelchangeComboInputrange)
END_MESSAGE_MAP()


// CLicelSettingsPage 消息处理程序


BOOL CLicelSettingsPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	m_ComboTrigType.AddString(_T("低电平触发"));
	m_ComboTrigType.AddString(_T("高电平触发"));
	m_ComboTrigType.SetCurSel(m_TrigType);

	m_ComboInputType.AddString(_T("500mV"));
	m_ComboInputType.AddString(_T("100mV"));
	m_ComboInputType.AddString(_T("20mV"));
	m_ComboInputType.SetCurSel(m_InputRangeType);

	CString strTemp;
	strTemp.Format(_T("%d"),m_DiscriminatorLevel);
	GetDlgItem(IDC_EDIT_DISCRIMINATOR_LEVEL)->SetWindowText(strTemp);

	strTemp.Format(_T("%d"),m_SampleFreq);
	GetDlgItem(IDC_EDIT_SAMPLE_FREQ)->SetWindowText(strTemp);

	strTemp.Format(_T("%d"),m_LaserFreq);
	GetDlgItem(IDC_EDIT_LASER_FREQ)->SetWindowText(strTemp);

	strTemp.Format(_T("%d"),m_CurvePointNum);
	GetDlgItem(IDC_EDIT_CURVE_POINT)->SetWindowText(strTemp);

	strTemp.Format(_T("%d"),m_CurveFreq);
	GetDlgItem(IDC_EDIT_CURVE_FREQ)->SetWindowText(strTemp);

	return TRUE;
}

void CLicelSettingsPage::LoadConfigs()
{
	CString iniFile = ((Cradar_wspApp *)AfxGetApp())->GetAppPath();
	iniFile += _T("configs\\licel.ini");

	m_TrigType = GetPrivateProfileInt(_T("summary"),_T("threshold_mode"),0,iniFile);
	m_InputRangeType = GetPrivateProfileInt(_T("summary"),_T("input_range_volts"),0,iniFile);
	m_DiscriminatorLevel = GetPrivateProfileInt(_T("summary"),_T("discriminator_level"),0,iniFile);
	m_SampleFreq = GetPrivateProfileInt(_T("summary"),_T("sample_freq"),20000000,iniFile);
	m_LaserFreq = GetPrivateProfileInt(_T("summary"),_T("laser_freq"),20,iniFile);
	m_CurvePointNum = GetPrivateProfileInt(_T("summary"),_T("curve_point"),2000,iniFile);
	m_CurveFreq = GetPrivateProfileInt(_T("summary"),_T("curve_freq"),1,iniFile);
}

void CLicelSettingsPage::SaveConfigs()
{
	CString iniFile = ((Cradar_wspApp *)AfxGetApp())->GetAppPath();
	iniFile += _T("configs\\licel.ini");

	CString strTemp;
	strTemp.Format(_T("%d"),m_TrigType);
	WritePrivateProfileString(_T("summary"),_T("threshold_mode"),strTemp,iniFile);

	strTemp.Format(_T("%d"),m_InputRangeType);
	WritePrivateProfileString(_T("summary"),_T("input_range_volts"),strTemp,iniFile);

	strTemp.Format(_T("%d"),m_DiscriminatorLevel);
	WritePrivateProfileString(_T("summary"),_T("discriminator_level"),strTemp,iniFile);

	strTemp.Format(_T("%d"),m_SampleFreq);
	WritePrivateProfileString(_T("summary"),_T("sample_freq"),strTemp,iniFile);

	strTemp.Format(_T("%d"),m_LaserFreq);
	WritePrivateProfileString(_T("summary"),_T("laser_freq"),strTemp,iniFile);

	strTemp.Format(_T("%d"),m_CurvePointNum);
	WritePrivateProfileString(_T("summary"),_T("curve_point"),strTemp,iniFile);

	strTemp.Format(_T("%d"),m_CurveFreq);
	WritePrivateProfileString(_T("summary"),_T("curve_freq"),strTemp,iniFile);
}

void CLicelSettingsPage::OnEnUpdateEditDiscriminatorLevel()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_DISCRIMINATOR_LEVEL)->GetWindowText(strTemp);
	m_DiscriminatorLevel = _ttof((LPTSTR)(LPCTSTR)strTemp);
}

void CLicelSettingsPage::OnEnUpdateEditSampleFreq()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_SAMPLE_FREQ)->GetWindowText(strTemp);
	m_SampleFreq = _ttof((LPTSTR)(LPCTSTR)strTemp);
}

void CLicelSettingsPage::OnEnUpdateEditLaserFreq()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_LASER_FREQ)->GetWindowText(strTemp);
	m_LaserFreq = _ttof((LPTSTR)(LPCTSTR)strTemp);
}

void CLicelSettingsPage::OnEnUpdateEditCurvePoint()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_CURVE_POINT)->GetWindowText(strTemp);
	m_CurvePointNum = _ttof((LPTSTR)(LPCTSTR)strTemp);
}

void CLicelSettingsPage::OnEnUpdateEditCurveFreq()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_CURVE_FREQ)->GetWindowText(strTemp);
	m_CurveFreq = _ttof((LPTSTR)(LPCTSTR)strTemp);
}

void CLicelSettingsPage::OnCbnSelchangeComboTrigtype()
{
	m_TrigType = m_ComboTrigType.GetCurSel();
}

void CLicelSettingsPage::OnCbnSelchangeComboInputrange()
{
	m_InputRangeType = m_ComboInputType.GetCurSel();
}
