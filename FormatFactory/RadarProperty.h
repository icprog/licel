#pragma once

#include <vector>
#include <fstream>
using namespace std;

class AFX_EXT_CLASS RadarProperty
{
public:
	RadarProperty(void);
	~RadarProperty(void);

public:
	vector<unsigned short> m_LenOfWaves;
	unsigned short m_ChannelCount;

public:
	void Store(ofstream& ofp);
	void Restore(ifstream& ifp);
};

