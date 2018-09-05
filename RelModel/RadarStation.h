#pragma once
#include <vector>

class __declspec(dllexport) CRadarStation
{
public:
	typedef struct tagPosition
	{
		double longitude;
		double latitude;
		unsigned short altitude;

		tagPosition()
		{
			longitude = 0.0;
			latitude = 0.0;
			altitude = 0;
		}
	}Position,*PTR_POSITION;

public:
	CRadarStation(void);
	~CRadarStation(void);

public:
	// 厂家名
	CString m_name;
	// 区站号
	CString m_id;
	// 型号
	CString m_model;
	// 通道数
	unsigned short m_chCount;
	// 波长
	std::vector<unsigned short> m_waves;
	// 位置
	Position m_position;

public:
	// 从配置文件中读取
	void Restore(CString configfile);
	// 保存至配置文件
	void Store(CString configfile);

public:
	static void SplitCString(CString strSource, CString ch,std::vector<CString> &dest);
};