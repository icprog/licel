#pragma once

class __declspec(dllexport) CRecordProperty
{
public:
	CRecordProperty();
	~CRecordProperty(void);

    // 探测模式类型
    typedef enum tagModeType
    {
        ModeType_Curve = 1, //扩线探测
        ModeType_Sum
    }ModeType;

public:
    // 探测模式
    ModeType m_ModeType
    // 采集开始时间
    CTime m_sTime;      
    // 采集结束时间
	CTime m_eTime; 
    // 俯仰角度              
	double m_FyAngle; 
    // 方位角度           
	double m_FwAngle;

public:
    // 设置当前时间为开始时间
    void SetCurrTimeForStart();
    // 设置当前时间为采集结束时间
    void SetCurrTimeForEnd();
};