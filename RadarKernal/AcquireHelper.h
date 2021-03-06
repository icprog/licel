#pragma once
#include <vector>

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
	//距离库
	double *m_pRange;

	// licel卡参数
	// 采集通道数量
	unsigned int m_ChannelNum;
	// 回波采样频率,Hz
	double m_SampleFreq;
	// 激光重复率,Hz
	unsigned int m_LaserFreq;
	// 廓线频率,Hz
	unsigned int m_CurveFreq;
	// 采样点数
	unsigned int m_CurvePoint;
	//输入信号范围
	unsigned int m_InputRange;
	//触发电平
	unsigned int m_Trig;
	//Discrimator Level
	unsigned int m_DiscriminatorLevel;

	//采集参数
	// 廓线累加周期,s
	unsigned int m_UpdatePeriod;
	
public:
	// 配置licel卡参数
	void ConfigLicelCard(CString configs);
	// 设置时间分辨率
	void SetDtScalerFactor(unsigned int period) { m_UpdatePeriod = period; }
	// 获取时间分辨率
	unsigned int GetDtScaleFactor() { return m_UpdatePeriod; }
	// 获取距离分辨率
	double GetHeightScaleFactor();
	// 获取采集通道数
	unsigned int GetChannelNum() { return m_ChannelNum; }
	// 获取距离库数
	unsigned int GetChannelDistCount() { return m_CurvePoint; }
	//获取距离库
	const double* GetRange() { return m_pRange; }
	//打开licel卡
	void OpenLicelCard(HWND hWnd);

protected:
	// 更新距离库
	void UpdateRange();
};

