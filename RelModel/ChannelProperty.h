#pragma once

class __declspec(dllexport) CChannelProperty
{
public:
    CChannelProperty();
    ~CChannelProperty();

    typedef enum tagAcqType
	{
		AcqType_AD = 0,
		AcqType_PC,
		AcqType_MIX,
		AcqType_Sum
	}AcqType;

	typedef enum tagWaveType
	{
		WaveType_P = 1,
		WaveType_S,
		WaveType_Laman,
		WaveType_Sum
	}WaveType;

    static char* AcqTypeString[AcqType_Sum];
	static char* WaveTypeString[WaveType_Sum];

public:
    // 通道号id
    unsigned int m_id;
    // 通道名称
	CString m_name;
    // 采集类型
	AcqType m_acqType;
    // 波长
	unsigned short m_waveLen;
    // 回波类型
	unsigned int m_waveType;
    // 距离分辨率
    double m_distResolution;
    // 盲区高度
	double m_deadZone;
    // 距离库数
    unsigned short m_distCount;

public:
	void Restore(CString config);
	void Store(CString config);
};