#pragma once

#include <fstream>
using namespace std;

class AFX_EXT_CLASS CRecordProperty
{
public:
	CRecordProperty();
	~CRecordProperty();
	
	//探测模式
    typedef enum tagModeType
    {
        ModeType_Curve = 1,//廓线探测
        ModeType_Sum
    }ModeType;

public:
	//探测模式
	ModeType m_ModeType;
    unsigned int m_sTotalSeconds;//开始秒计数
	unsigned int m_eTotalSeconds;//结束秒计数 
	unsigned short m_TotalDays;//儒略日
	double m_FyAngle;//俯仰角 
	double m_FwAngle;//方位角

public:
    void SetCurrForStartTime();
    void SetCurrForEndTime();
	void GetStartTime(CTime& t);
	void GetEndTime(CTime& t);

public:
	void Store(ofstream& ofp);
	void Restore(ifstream& ifp);
};