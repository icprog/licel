#pragma once
#include <vector>
#include <fstream>
using namespace std;

class  CRadarStation
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
	//ÇøÕ¾ºÅ
	CString m_id;
	//Î»ÖÃ
	Position m_position;

public:
	void Restore(ifstream& ifp);
	void Store(ofstream* ofp);

public:
	static void SplitCString(CString strSource, CString ch,std::vector<CString> &dest);
};