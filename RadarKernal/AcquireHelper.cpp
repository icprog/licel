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

	m_ChannelNum = 3;
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

void CAcquireHelper::ConfigLicelCard(CString configs)
{
	m_SampleFreq = GetPrivateProfileInt(_T("summary"),_T("sample_freq"),20000000,configs);
	m_LaserFreq = GetPrivateProfileInt(_T("summary"),_T("laser_freq"),20,configs);
	m_CurveFreq = GetPrivateProfileInt(_T("summary"),_T("curve_freq"),1,configs);
	m_CurvePoint = GetPrivateProfileInt(_T("summary"),_T("curve_point"),2000,configs);
	m_InputRange = GetPrivateProfileInt(_T("summary"),_T("input_range_volts"),0,configs);
	m_Trig = GetPrivateProfileInt(_T("summary"),_T("threshold_mode"),0,configs);
	m_DiscriminatorLevel = GetPrivateProfileInt(_T("summary"),_T("discriminator_level"),0,configs);

	UpdateRange();
}

void CAcquireHelper::UpdateRange()
{
	if(m_pRange != NULL)
		delete m_pRange;

	m_pRange = new double[m_CurvePoint];
	double heightResolution = GetHeightScaleFactor();
	for(unsigned int i=0;i<m_CurvePoint;i++)
		m_pRange[i] = (i + 1) * heightResolution;
}

double CAcquireHelper::GetHeightScaleFactor()
{
	double heightScaleFactor = m_LIGHT_SPEED / m_SampleFreq / 2.0;
	return heightScaleFactor;
}

void CAcquireHelper::OpenLicelCard(HWND hWnd)
{
	LicelAcq_Start(m_LaserFreq / m_CurveFreq,m_CurvePoint,m_Trig,m_InputRange,m_DiscriminatorLevel,hWnd);
}

