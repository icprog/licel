#pragma once
#include <vector>

class AFX_EXT_CLASS CRadarStation
{
public:
	typedef struct tagPosition
	{
		double longitude;
		double latitude;
		double altitude;

		tagPosition()
		{
			longitude = 0.0;
			latitude = 0.0;
			altitude = 0.0;
		}
	}Position,*PTR_POSITION;

public:
	CRadarStation(void);
	~CRadarStation(void);

public:
	CString m_name;
	CString m_id;
	CString m_model;
	unsigned short m_chs;
	std::vector<unsigned short> m_waves;
	Position m_position;

public:
	void restore(CString configfile);
	void store(CString configfile);

public:
	static void SplitCString(CString strSource, CString ch,std::vector<CString> &dest);
};