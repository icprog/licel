#pragma once

#include <vector>
#include "afxcmn.h"
using namespace std;

class CStationSettingsPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(CStationSettingsPage)

public:
	CStationSettingsPage();
	virtual ~CStationSettingsPage();

	enum { IDD = IDD_STATIONSETTINGSDLG };

protected:
	CString m_Factory;
	CString m_StationID;
	CString m_Module;
	double m_Longitude;
	double m_Latitude;
	double m_Altitude;
	vector<unsigned short> m_LenOfWave;
	vector<unsigned char> m_StateOfWave;
	CListCtrl m_ChannelList;

	CXTPPropertyPageStaticCaption m_Captions[2];

	void LoadConfigs();
	static void SplitString(CString &str,vector<CString> &vects);

public:
	void SaveConfigs();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
};
