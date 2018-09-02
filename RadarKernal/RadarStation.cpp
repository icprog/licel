#include "StdAfx.h"
#include "RadarStation.h"


CRadarStation::CRadarStation(void)
{
	m_name = _T("");
	m_id = _T("");
	m_model = _T("");
	m_chs = 0;
}


CRadarStation::~CRadarStation(void)
{

}

void CRadarStation::restore(CString configfile)
{
	unsigned char cBuf[_MAX_PATH];
	memset(cBuf,0,_MAX_PATH);
	// 厂家名
	GetPrivateProfileString(_T("summary"),_T("factory"),_T(""),(LPWSTR)cBuf,_MAX_PATH,configfile);
	m_name.Empty();
	m_name.Append((LPWSTR)cBuf);

	memset(cBuf,0,_MAX_PATH);
	// 区站号
	GetPrivateProfileString(_T("summary"),_T("station"),_T(""),(LPWSTR)cBuf,_MAX_PATH,configfile);
	m_name.Empty();
	m_id.Append((LPWSTR)cBuf);

	memset(cBuf,0,_MAX_PATH);
	// 型号
	GetPrivateProfileString(_T("summary"),_T("model"),_T(""),(LPWSTR)cBuf,_MAX_PATH,configfile);
	m_name.Empty();
	m_model.Append((LPWSTR)cBuf);

	// 通道数
	m_chs = GetPrivateProfileInt(_T("summary"),_T("channels"),0,configlefile);

	CString wave_temp;
	memset(cBuf,0,_MAX_PATH);
	// 波长
	GetPrivateProfileString(_T("laser"),_T("wave_len"),_T(""),(LPWSTR)cBuf,_MAX_PATH,configfile);
	wave_temp.Empty();
	wave_temp.Append((LPWSTR)cBuf);

	vector<CString> waveString;
	SplitCString(wave_temp,_T(","),waveString);

	std::vector<CString>::iterator it = waveString.begin();
	while(it != waveString.end())
	{
		m_waves.push_back(_ttof((*it)));
		it++;
	}

	CString positionString;
	// 站点位置
	// 经度
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("position"),_T("longitude"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,configfile);
	positionString.Empty();
	positionString.Append((LPWSTR)cBuf);
	m_position.m_longitude = _ttof(positionString);

	// 纬度
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("position"),_T("latitude"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,configfile);
	positionString.Empty();
	positionString.Append((LPWSTR)cBuf);
	m_position.m_latitude = _ttof(positionString);

	// 高度
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("position"),_T("altitude"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,configfile);
	positionString.Empty();
	positionString.Append((LPWSTR)cBuf);
	m_position.altitude = _ttof(positionString);
}

void CRadarStation::store(CString configfile)
{

}

void CRadarStation::SplitCString(CString strSource, CString ch,std::vector<CString> &dest)
{
	int iPos = 0;
    CString strTmp;
    strTmp = strSource.Tokenize(ch,iPos);

	while(strTmp.Trim() != _T(""))
	{
		dest.push_back(strTmp);
		strTmp = strSource.Tokenize(ch,iPos);
	}
}