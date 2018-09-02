#include "StdAfx.h"
#include "DatRecord.h"

double gb_tmpVal[10];
char gb_tmpArray[1024*1024*2];
string gb_tmpString;
int gb_pos,gb_iStart,gb_iEnd;
int gb_line=0;
CString gb_strPropertyName,gb_strPropertyVal;
vector<CString> gb_colStr;

CDatRecord::CDatRecord()
{
	m_pSamples = NULL;
	m_pExtinctionCoefficient = NULL;
	m_pDeplorRatio = NULL;
	m_pCloud = NULL;
	m_pOpticDepth= NULL;
}

CDatRecord::~CDatRecord(void)
{
	Clear();
}

BOOL CDatRecord::LoadItemSrc(CString file)
{
	ifstream fi(file);
	if(!fi.is_open())
		return FALSE;
	if(!ParserHeader(fi))
		return FALSE;
	if(!ParserDatas(fi))
		return FALSE;

	fi.close();
	return TRUE;
}

void CDatRecord::Serial2File(CString file)
{
	ofstream f;
	f.open(file,std::ios_base::binary);

	
	
	f.close();
}

BOOL CDatRecord::LoadItemHeader(CString file)
{
	ifstream fi(file);
	if(!fi.is_open())
		return FALSE;
	if(!ParserHeader(fi))
		return FALSE;
	fi.close();
	return TRUE;
}

BOOL CDatRecord::LoadItemData(CString file)
{
	ifstream fi(file);
	if(!fi.is_open())
		return FALSE;
	if(!ParserDatas(fi))
		return FALSE;
	fi.close();
	return TRUE;
}

BOOL CDatRecord::ParserHeader(ifstream& fi)
{
	while(!fi.eof())
	{
		getline(fi,gb_tmpString);
		if(gb_tmpString != "#Property:")
			continue;
		else break;
	}
	gb_iStart = fi.tellg();
	while(!fi.eof())
	{
		getline(fi,gb_tmpString);
		if(gb_tmpString != "#Samples:")
		{
			gb_line++;
			continue;
		}
		else break;
	}
	gb_iEnd = fi.tellg();
	fi.seekg(gb_iStart,ios::beg);

	while((gb_line--)>=0)
	{
		getline(fi,gb_tmpString);
		CString strContent(gb_tmpString.c_str());
		gb_pos = 0;
		gb_strPropertyName = strContent.Tokenize(_T(":"),gb_pos);
		if(gb_strPropertyName.Trim() == _T(""))
			continue;
		gb_strPropertyVal = strContent.Tokenize(_T(":"),gb_pos);
		gb_strPropertyVal.Trim();
		if(gb_strPropertyName == _T("capture mode"))
		{
			if(gb_strPropertyVal == _T("Fixed"))
				m_Mode = 0;
			else if(gb_strPropertyVal == _T("PPI"))
				m_Mode = 1;
			else if(gb_strPropertyVal == _T("RHI"))
				m_Mode = 2;
			else m_Mode = -1;
		}
		else if(gb_strPropertyName == _T("fy"))
		{
			m_FyAngle = _ttof((LPTSTR)(LPCTSTR)gb_strPropertyVal);
		}
		else if(gb_strPropertyName == _T("fw"))
		{
			m_FwAngle = _ttof((LPTSTR)(LPCTSTR)gb_strPropertyVal);
		}
		else if(gb_strPropertyName == _T("sample freq(MHz)"))
		{
			m_SampleFreq = _ttof((LPTSTR)(LPCTSTR)gb_strPropertyVal) * 1000000.0;
		}
		else if(gb_strPropertyName == _T("channels"))
		{
			m_Chs = _ttof((LPTSTR)(LPCTSTR)gb_strPropertyVal);
		}
		else if(gb_strPropertyName == _T("frame length"))
		{
			m_SampleNum = _ttof((LPTSTR)(LPCTSTR)gb_strPropertyVal);	
		}
		else if(gb_strPropertyName == _T("frame period(s)"))
		{
			m_FramePeriod = _ttof((LPTSTR)(LPCTSTR)gb_strPropertyVal);	
		}
		else if(gb_strPropertyName == _T("statiom"))
		{
			m_Station = gb_strPropertyVal;	
		}
		else if(gb_strPropertyName == _T("conductor"))
		{
			m_Conductor = gb_strPropertyVal;	
		}
		else if(gb_strPropertyName == _T("comment"))
		{
			m_Comment = gb_strPropertyVal;	
		}
	}
	fi.seekg(gb_iEnd,ios::beg);
	return TRUE;
}

BOOL CDatRecord::ParserDatas(ifstream& fi)
{
	while(!fi.eof())
	{
		getline(fi,gb_tmpString);
		if(gb_tmpString != "#Samples:")
			continue;
		else break;
	}
	
	if(!fi.eof())
		getline(fi,gb_tmpString);
	else return FALSE;
	
	CString strContent(gb_tmpString.c_str());
	gb_pos = 0;
	gb_strPropertyVal = strContent.Tokenize(_T(" "),gb_pos);
	gb_colStr.clear();
	while(gb_strPropertyVal.Trim() != _T(""))
	{
		gb_colStr.push_back(gb_strPropertyVal);
		gb_strPropertyVal = strContent.Tokenize(_T(" "),gb_pos);
	}
	if(gb_colStr.size() < 2)
		return FALSE;
	
	gb_pos = gb_colStr.size();
	m_pSamples = new CSample(gb_pos,FALSE);
		
	stringstream gb_tmpStringStream;
	gb_tmpStringStream<<gb_tmpString;
	for(gb_line=0;gb_line<gb_pos;gb_line++)
		gb_tmpStringStream >> gb_tmpVal[gb_line];
	m_pSamples->Add(gb_tmpVal,gb_pos);
	
	gb_iStart = fi.tellg();
	fi.seekg(0,ios::end);
	gb_iEnd = fi.tellg();
	gb_iEnd -= gb_iStart;
	fi.seekg(gb_iStart,ios::beg);
	fi.read(gb_tmpArray,gb_iEnd);
	gb_tmpStringStream.clear();
	gb_tmpStringStream<<gb_tmpArray;
	while(gb_tmpStringStream.good())
	{
		gb_iStart = gb_tmpStringStream.tellg();
		if(gb_iEnd - gb_iStart < 10)
			break;
		for(gb_line=0;gb_line<gb_pos;gb_line++)
			gb_tmpStringStream >> gb_tmpVal[gb_line];
		m_pSamples->Add(gb_tmpVal,gb_pos);
		gb_iEnd --;
	}

	return TRUE;
}

void CDatRecord::Clear()
{
	if(m_pSamples != NULL)
	{
		m_pSamples->Release();
		m_pSamples = NULL;
	}
	if(m_pExtinctionCoefficient != NULL)
	{
		m_pExtinctionCoefficient->Release();
		m_pExtinctionCoefficient = NULL;
	}
	if(m_pDeplorRatio != NULL)
	{
		m_pDeplorRatio->Release();
		m_pDeplorRatio = NULL;
	}
	if(m_pCloud != NULL)
	{
		m_pCloud->Release();
		m_pCloud = NULL;
	}
	if(m_pOpticDepth != NULL)
	{
		m_pOpticDepth->Release();
		m_pOpticDepth = NULL;
	}
}