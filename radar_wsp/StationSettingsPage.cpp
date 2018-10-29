#include "stdafx.h"
#include "radar_wsp.h"
#include "StationSettingsPage.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CStationSettingsPage, CXTPPropertyPage)

CStationSettingsPage::CStationSettingsPage()
	: CXTPPropertyPage(CStationSettingsPage::IDD)
{
	LoadConfigs();
}

CStationSettingsPage::~CStationSettingsPage()
{
}

void CStationSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHANNEL, m_ChannelList);
}


BEGIN_MESSAGE_MAP(CStationSettingsPage, CXTPPropertyPage)
END_MESSAGE_MAP()


BOOL CStationSettingsPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	m_Captions[0].SubclassDlgItem(IDC_STATIC_STATION,this);
	SetResize(IDC_STATIC_STATION , XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

	m_Captions[1].SubclassDlgItem(IDC_STATIC_RADAR,this);
	SetResize(IDC_STATIC_RADAR , XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

	GetDlgItem(IDC_EDIT_FACTORY)->SetWindowText(m_Factory);
	GetDlgItem(IDC_EDIT_ID)->SetWindowText(m_StationID);
	GetDlgItem(IDC_EDIT_MODULE)->SetWindowText(m_Module);

	CString strTemp;
	strTemp.Format(_T("%.6f"),m_Longitude);
	GetDlgItem(IDC_EDIT_LONGITUDE)->SetWindowText(strTemp);
	strTemp.Format(_T("%.6f"),m_Latitude);
	GetDlgItem(IDC_EDIT_LATITUDE)->SetWindowText(strTemp);
	strTemp.Format(_T("%.6f"),m_Altitude);
	GetDlgItem(IDC_EDIT_ALTITUDE)->SetWindowText(strTemp);

	DWORD dwStyle = m_ChannelList.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
	m_ChannelList.SetExtendedStyle(dwStyle);

	m_ChannelList.InsertColumn(0,_T("Í¨µÀÐòºÅ"),LVCFMT_LEFT, 50);
	m_ChannelList.InsertColumn(1,_T("²¨³¤"),LVCFMT_LEFT, 120);
	m_ChannelList.InsertColumn(2,_T("Æ«ÕñÌ¬"),LVCFMT_LEFT, 120);

	int chs = min(m_LenOfWave.size(),m_StateOfWave.size());
	for(int i=0;i<chs;i++)
	{
		strTemp.Format(_T("%d"),i+1);
		int nRow = m_ChannelList.InsertItem(i, strTemp);
		strTemp.Format(_T("%dnm"),m_LenOfWave.at(i));
		m_ChannelList.SetItemText(nRow,1,strTemp);

		CString stateOfWaveStr(CProductHelper::WAVE_STATE[m_StateOfWave.at(i)]);
		m_ChannelList.SetItemText(nRow,2,stateOfWaveStr);
	}

	return TRUE;
}

void CStationSettingsPage::LoadConfigs()
{
	CString iniFile = ((Cradar_wspApp *)AfxGetApp())->GetAppPath();
	iniFile += _T("configs\\station.ini");

	unsigned char cBuf[_MAX_PATH];
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("station"),_T("factory"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	m_Factory.Append((LPWSTR)cBuf);

	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("station"),_T("id"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	m_StationID.Append((LPWSTR)cBuf);

	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("station"),_T("module"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	m_Module.Append((LPWSTR)cBuf);

	CString strTemp;
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("station"),_T("longitude"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	strTemp.Append((LPWSTR)cBuf);
	m_Longitude = _ttof((LPTSTR)(LPCTSTR)strTemp);

	memset(cBuf,0,_MAX_PATH);
	strTemp.Empty();
	GetPrivateProfileString(_T("station"),_T("latitude"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	strTemp.Append((LPWSTR)cBuf);
	m_Latitude = _ttof((LPTSTR)(LPCTSTR)strTemp);

	memset(cBuf,0,_MAX_PATH);
	strTemp.Empty();
	GetPrivateProfileString(_T("station"),_T("altitude"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	strTemp.Append((LPWSTR)cBuf);
	m_Altitude = _ttof((LPTSTR)(LPCTSTR)strTemp);

	memset(cBuf,0,_MAX_PATH);
	strTemp.Empty();
	GetPrivateProfileString(_T("radar"),_T("wave_len"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	strTemp.Append((LPWSTR)cBuf);

	vector<CString> vects;
	SplitString(strTemp,vects);

	vector<CString>::iterator it = vects.begin();
	while(it != vects.end())
	{
		m_LenOfWave.push_back(_ttof((LPTSTR)(LPCTSTR)(*it)));
		it++;
	}

	memset(cBuf,0,_MAX_PATH);
	strTemp.Empty();
	GetPrivateProfileString(_T("radar"),_T("wave_state"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	strTemp.Append((LPWSTR)cBuf);

	vects.clear();
	SplitString(strTemp,vects);
	it = vects.begin();
	while(it != vects.end())
	{
		m_StateOfWave.push_back(_ttof((LPTSTR)(LPCTSTR)(*it)));
		it++;
	}
}

void CStationSettingsPage::SaveConfigs()
{

}

void CStationSettingsPage::SplitString(CString &str,vector<CString> &vects)
{
	int iPos = 0;
    CString strTmp;
	CString strline = str;
    strTmp = strline.Tokenize(_T(","),iPos);

	while(strTmp.Trim() != _T(""))
	{
		vects.push_back(strTmp);
		strTmp = strline.Tokenize(_T(","),iPos);
	}
}