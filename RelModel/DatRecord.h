#pragma once
#include "Sample.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class __declspec(dllexport) CDatRecord
{
public:
	CDatRecord();
	~CDatRecord(void);
	
public:
	unsigned long m_Id;
	CTime m_Time;                //采集时间	
	short m_Mode;                //采集模式
	double m_FyAngle;            //俯仰角度
	double m_FwAngle;            //方位角度
	double m_SampleFreq;         //采样频率
	unsigned short m_Chs;        //通道数
	unsigned short m_SampleNum;  //样本个数
	unsigned short m_FramePeriod;//累计周期
	CString m_Station;           //站点
	CString m_Conductor;         //操作者
	CString m_Comment;           //备注

	CSample *m_pSamples;              //原始信号
	CSample *m_pRegular;              //预处理后信号
	CSample *m_pBackScatter;          //后向散射系数
	CSample *m_pExtinctionCoefficient;//消光系数
	CSample *m_pDeplorRatio;          //退偏振比

	CSample *m_pCloud;                //云高
	CSample *m_pOpticDepth;           //光学厚度
	CSample *m_pBoundary;             //边界层高度
	CSample *m_pVisiblity;            //垂直能见度
	CSample *m_pMix;                  //混合层高度
	CSample *m_pGasoloidPotency;      //气溶胶浓度

public:
	void Clear();
};

