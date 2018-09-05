#include "StdAfx.h"
#include "AcquireHelper.h"

const double CAcquireHelper::m_LIGHT_SPEED = 299.792458*1000000;
const char* CAcquireHelper::AcquirePeriod_Caption[] = {"10秒","30秒","1分钟","2分钟","5分钟","10分钟","30分钟","1小时","2小时"};
const unsigned int CAcquireHelper::AcquirePeriod_Value[] = {10,
											                30,
															60,
															120,
															300,
															600,
															1800,
															3600,
															7200};

CAcquireHelper::CAcquireHelper(void)
{
	m_pRange = NULL;

	m_ChannelNum = 6;
	m_CurveFreq = 1;
	m_SampleFreq = 20000000;
	m_LaserFreq = 20;
	m_CurvePoint = 2000;

	m_UpdatePeriod = 10;
	UpdateRange();
}

CAcquireHelper::~CAcquireHelper(void)
{
}

BOOL CAcquireHelper::ConfigLicelCard(CString configs)
{
	m_ChannelNum = GetPrivateProfileInt(_T("summary"),_T("channel_count"),6,configs);
	m_SampleFreq = GetPrivateProfileInt(_T("summary"),_T("sample_freq"),20000000,configs);
	m_LaserFreq = GetPrivateProfileInt(_T("summary"),_T("laser_freq"),20,configs);
	m_CurveFreq = GetPrivateProfileInt(_T("summary"),_T("curve_freq"),1,configs);
	m_CurvePoint = GetPrivateProfileInt(_T("summary"),_T("curve_point"),2000,configs);

	UpdateRange();
}

BOOL CAcquireHelper::ConfigStationProperty(CString configs)
{
	m_RadarStation.Restore(configs);
}

BOOL CAcquireHelper::ConfigRecordProperty(CString configs)
{
	m_UpdatePeriod = GetPrivateProfileInt(_T("capture"),_T("period"),10,configs);
}

BOOL CAcquireHelper::ConfigChannelProperty(CString configs)
{
	CString sectStr;
	for(int i=0;i<m_ChannelNum;i++)
	{
		CChannelProperty *pChannelProperty = new CChannelProperty();
		m_ChannelPropertys.push_back(pChannelProperty);
		pChannelProperty->m_id = i+1;
		pChannelProperty->m_distResolution = GetHeightScaleFactor();
		pChannelProperty->m_distCount = m_CurvePoint;
		pChannelProperty->Restore(configs);		
	}
}

void CAcquireHelper::UpdateRange()
{
	if(m_pRange != NULL)
		delete m_pRange;

	m_pRange = new double[5000];
	double heightResolution = GetHeightScaleFactor();
	for(unsigned int i=0;i<5000;i++)
		m_pRange[i] = (i + 1) * heightResolution;
}

double CAcquireHelper::GetHeightScaleFactor()
{
	double heightScaleFactor = m_LIGHT_SPEED / m_SampleFreq / 2.0;
	return heightScaleFactor;
}

void CAcquireHelper::OpenLicelCard(HWND hWnd)
{
	LicelAcq_Start(m_LaserFreq / m_CurveFreq,m_CurvePoint,hWnd);
}

