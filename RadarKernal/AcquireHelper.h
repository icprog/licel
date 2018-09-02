#pragma once
class AFX_EXT_CLASS CAcquireHelper
{
public:
	CAcquireHelper(void);
	~CAcquireHelper(void);

	friend class CProductHelper;

	typedef enum tagAcquirePeriod_Type
	{
		AcquirePeriod_10sec = 0,
		AcquirePeriod_30sec,
		AcquirePeriod_1min,
		AcquirePeriod_2min,
		AcquirePeriod_5min,
		AcquirePeriod_10min,
		AcquirePeriod_30min,
		AcquirePeriod_1hour,
		AcquirePeriod_2hour,
		AcquirePeriod_Sum
	}AcquirePeriod_Type;

	const static char* AcquirePeriod_Caption[AcquirePeriod_Sum];
	const static unsigned int AcquirePeriod_Value[AcquirePeriod_Sum];
	const static double m_LIGHT_SPEED;//光速(m/s)
	
protected:
	double *m_pRange;
	//回波信号通道数量
	unsigned int m_ChannelNum;
	//高度分辨率,m
	double m_HeightScaleFactor;
	//回波采样频率,Hz
	double m_SampleFreq;
	//廓线采样点
	unsigned int m_CurvePoints;
	//廓线累加周期,s
	unsigned int m_UpdatePeriod;
	//廓线频率,Hz
	unsigned int m_CurveFreq;
	//激光重复率,Hz
	unsigned int m_LaserFreq;
	
public:
	void SetDtScalerFactor(unsigned int period) { m_UpdatePeriod = period; }
	unsigned int GetDtScaleFactor() { return m_UpdatePeriod; }
	double GetHeightScaleFactor() { return m_HeightScaleFactor; }
	void UpdateHeightScaleFactor();
	unsigned int GetCurvePointNum() { return m_CurvePoints; }
	unsigned int GetChannelNum() { return m_ChannelNum; }
	static double CalcHeightResolution(double sampleFreq);

	void OpenLicelCard(HWND hWnd);

protected:
	void UpdateRange();
};

