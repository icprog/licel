#include "StdAfx.h"
#include "AxisConfigHelper.h"


CAxisConfigHelper::CAxisConfigHelper(void)
{
}


CAxisConfigHelper::~CAxisConfigHelper(void)
{
}

void CAxisConfigHelper::ConfigFromFile(CString file)
{
	m_FilePath  = file;

	CString strTemp;
	CString strSectionName;
	unsigned char cBuf[_MAX_PATH];

	for(int j=0;j<CProductHelper::ProductChannelType_Sum;j++)
	{
		for(int i=0;i<CProductHelper::ProductType_Sum;i++)
		{
			CString productName(CProductHelper::PRODUCT_NAME[i]);
			strSectionName.Format(_T("_ch%d"),j+1);
			strSectionName = productName + strSectionName;
			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("min"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].min = _ttof((LPTSTR)(LPCTSTR)strTemp);

			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("max"),_T("100000.0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].max = _ttof((LPTSTR)(LPCTSTR)strTemp);

			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("log_t"),_T("0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].log_t = _ttof((LPTSTR)(LPCTSTR)strTemp);

			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("level_min"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].level_min = _ttof((LPTSTR)(LPCTSTR)strTemp);

			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("level_max"),_T("100000.0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].level_max = _ttof((LPTSTR)(LPCTSTR)strTemp);

			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("level_visible"),_T("0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].level_visible = _ttof((LPTSTR)(LPCTSTR)strTemp);

			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("log_c"),_T("0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].log_c = _ttof((LPTSTR)(LPCTSTR)strTemp);

			strSectionName.Format(_T("_default_ch%d"),j+1);
			strSectionName = productName + strSectionName;

			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("min"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].default_min = _ttof((LPTSTR)(LPCTSTR)strTemp);

			strTemp.Empty();
			memset(cBuf,0,_MAX_PATH);
			GetPrivateProfileString(strSectionName,_T("max"),_T("100000.0"),(LPWSTR)cBuf,_MAX_PATH,file);
			strTemp.Append((LPWSTR)cBuf);
			m_AxisSettings[j][i].default_max = _ttof((LPTSTR)(LPCTSTR)strTemp);
		}
	}
}

void CAxisConfigHelper::SaveModify(CProductHelper::ProductType productType,unsigned int ch)
{
	PTR_AxisSettings pAxisSettings = &(m_AxisSettings[ch][productType]);
	CString productName(CProductHelper::PRODUCT_NAME[productType]);
	CString strTemp;

	CString strSectionName;
	strSectionName.Format(_T("_ch%d"),ch+1);
	strSectionName = productName + strSectionName;

	strTemp.Format(_T("%.6f"),pAxisSettings->min);
	WritePrivateProfileString(strSectionName,_T("min"),strTemp,m_FilePath);

	strTemp.Format(_T("%.6f"),pAxisSettings->max);
	WritePrivateProfileString(strSectionName,_T("max"),strTemp,m_FilePath);

	strTemp.Format(_T("%d"),pAxisSettings->log_t);
	WritePrivateProfileString(strSectionName,_T("log_t"),strTemp,m_FilePath);

	strTemp.Format(_T("%.6f"),pAxisSettings->level_min);
	WritePrivateProfileString(strSectionName,_T("level_min"),strTemp,m_FilePath);

	strTemp.Format(_T("%.6f"),pAxisSettings->level_max);
	WritePrivateProfileString(strSectionName,_T("level_max"),strTemp,m_FilePath);

	strTemp.Format(_T("%d"),pAxisSettings->level_visible);
	WritePrivateProfileString(strSectionName,_T("level_visible"),strTemp,m_FilePath);

	strTemp.Format(_T("%d"),pAxisSettings->log_c);
	WritePrivateProfileString(strSectionName,_T("log_c"),strTemp,m_FilePath);
}

CAxisConfigHelper::PTR_AxisSettings CAxisConfigHelper::GetProductConfig(CProductHelper::ProductType productType,unsigned int ch)
{
	return &(m_AxisSettings[ch][productType]);
}
