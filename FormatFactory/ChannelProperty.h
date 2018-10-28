#pragma once

class AFX_EXT_CLASS CChannelProperty
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
    unsigned int m_id;
	CString m_name;
	AcqType m_acqType;
	unsigned short m_waveLen;
	unsigned int m_waveType;
    double m_distResolution;
	double m_deadZone;
    unsigned short m_distCount;

public:
	int Restore(CString config);
	void Store(CString config);
};