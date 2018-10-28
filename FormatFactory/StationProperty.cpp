#include "StdAfx.h"
#include "StationProperty.h"

CRadarStation::CRadarStation(void)
{
	m_id = _T("");
}


CRadarStation::~CRadarStation(void)
{

}

void CRadarStation::Restore(ifstream& ifp)
{
	
}

void CRadarStation::Store(ofstream* ofp)
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