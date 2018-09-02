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
	m_CurvePoints =2000;
	m_SampleFreq = 20000000;
	m_LaserFreq = 20;
	m_UpdatePeriod = 10;
	UpdateHeightScaleFactor();
}

CAcquireHelper::~CAcquireHelper(void)
{
}

void CAcquireHelper::UpdateRange()
{
	if(m_pRange != NULL)
		delete m_pRange;

	m_pRange = new double[m_CurvePoints];
	for(unsigned int i=0;i<m_CurvePoints;i++)
		m_pRange[i] = i*m_HeightScaleFactor;
}

void CAcquireHelper::UpdateHeightScaleFactor()
{
	m_HeightScaleFactor = CalcHeightResolution(m_SampleFreq);
	UpdateRange();
}

double CAcquireHelper::CalcHeightResolution(double sampleFreq)
{
	return m_LIGHT_SPEED / sampleFreq  / 2.0;
}

void CAcquireHelper::OpenLicelCard(HWND hWnd)
{
	LicelAcq_Start(m_LaserFreq / m_CurveFreq,m_CurvePoints,hWnd);
}

